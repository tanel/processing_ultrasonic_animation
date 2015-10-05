/*global $:false, window:false, Chart:false, document:false, moment:false */
"use strict";
$(function () {
    // Configuration
    window.settings = {
        ajaxIntervalMillis: 2000,
        chartRotateIntervalMillis: 5000,
        dateFormat: "DD.MM.YYYY",
        jsonDateFormat: "YYYY-MM-DD",
        killColor: "#F7464A",
        saveColor: "#46BFBD",
    };

    // we'll keep data here
    window.stats = {};

    // callback func for ajax timer
    window.ajaxFunc = function () {
        console.log("updating data using AJAX");
        $.get("http://localhost:8000/gamestats.json", function (data) {
            console.log(data);
            window.stats = data;
            window.setTimeout(window.ajaxFunc, window.settings.ajaxIntervalMillis);
        });
    };

    // the chart currently being displayed,
    // since we start with empty page, the value is -1.
    window.currentChart = 0;

    // callback func chart rotating
    window.chartRotateFunc = function () {
        console.log("rotating charts");

        window.ensureData();

        window.displayDates();

        window.displayTotals();

/*
        window.currentChart = window.currentChart + 1;
        if (window.currentChart > 3) {
            window.currentChart = 0;
        }
*/

        // Hide visible chart(s)
        $('.stats').hide();

        // show the current chart
        $('#chart' + String(window.currentChart)).show();

        // draw its internal contents.
        // (the indices 1 and 2 respond to chart ID's in HTML)
        if (1 === window.currentChart) {
            window.displayBarChart();
        } else if (2 === window.currentChart) {
            window.displayPieChart();
        }
    };

    // Rotate charts with N millis interval (see settings)
    window.chartRotateTimer = window.setInterval(window.chartRotateFunc, window.settings.chartRotateIntervalMillis);

    window.ensureData = function () {
        if (!window.stats.total) {
            window.stats.total = {};
        }
        if (!window.stats.history) {
            window.stats.history = {};
        }
    };

    window.displayBarChart = function () {
        var bardata = {
            labels: [],
            datasets: [
                {
                    label: "Saves",
                    data: [],
                    backgroundColor: window.settings.saveColor,
                },
                {
                    label: "Kills",
                    data: [],
                    backgroundColor: window.settings.killColor,
                }
            ],
        },
            barctx = document.getElementById("barchart").getContext("2d");
        $.each(window.stats.history, function (key, val) {
            bardata.labels.push(key);
            bardata.datasets[0].data.push(val.saves);
            bardata.datasets[1].data.push(val.kills);
        });
        window.barChart = new Chart(barctx, {
            type: 'bar',
            data: bardata,
            options: {
                responsive: false,
            }
        });
    };

    // Update the total numbers, which are included in multiple charts
    window.displayTotals = function () {
        window.ensureData();
        $(".total_saves").text(window.stats.total.saves);
        $(".total_kills").text(window.stats.total.kills);

        // find todays stats
        var today = moment().format(window.settings.jsonDateFormat),
            todaysResults = window.stats.history[today];
        if (todaysResults) {
            $(".today_saves").text(todaysResults.saves);
            $(".today_kills").text(todaysResults.kills);
        }
    };

    // Until we have real data from backend, use today's date
    window.startDate = moment();
    window.endDate = moment();

    // Update the dates, which are visible in multiple charts
    window.displayDates = function () {
        var today = moment();
        $(".today").text(today.format(window.settings.dateFormat));

        // find the start and end date range of available data
        $.each(window.stats.history, function (key) {
            var date = moment(key);
            if (date.isBefore(window.startDate)) {
                window.startDate = moment(date);
            }
            if (date.isAfter(window.endDate)) {
                window.endDate = moment(date);
            }
        });
        $(".start_date").text(window.startDate.format(window.settings.dateFormat));
        $(".end_date").text(window.endDate.format(window.settings.dateFormat));
    };

    window.displayPieChart = function () {
        window.ensureData();
        var piedata = {
                labels: [
                    "UUDISHIMU / CURIOSITY",
                    "HUMAANSUS / HUMANITY",
                ],
                datasets: [{
                    data: [
                        window.stats.total.kills,
                        window.stats.total.saves,
                    ],
                    backgroundColor: [
                        window.settings.killColor,
                        window.settings.saveColor,
                    ],
                }],
            },
            piectx = document.getElementById("piechart").getContext("2d");
        window.pieChart = new Chart(piectx, {
            type: 'pie',
            data: piedata,
            options: {
                responsive: false,
            }
        });
    };

    console.log("initialized");

    window.chartRotateFunc();

    // Start by updating data
    window.ajaxFunc();
});

