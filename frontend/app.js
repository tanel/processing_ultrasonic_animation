/*global $:false, window:false, Chart:false, document:false */
"use strict";
$(function () {
    // Configuration
    window.settings = {
        ajaxIntervalMillis: 10000,
        chartRotateIntervalMillis: 5000,
    };

    // we'll keep data here
    window.stats = {};

    // callback func for ajax timer
    window.ajaxFunc = function () {
        console.log("updating data using AJAX");
        $.get("http://localhost:8000/gamestats.json", function (data) {
            console.log(data);
            window.stats = data;
        });
    };

    // update data with a given interval
    window.ajaxTimer = window.setInterval(window.ajaxFunc, window.settings.ajaxIntervalMillis);

    // the chart currently being displayed,
    // since we start with empty page, the value is -1.
    window.currentChart = -1;

    // callback func chart rotating
    window.chartRotateFunc = function () {
        console.log("rotating charts");

        window.ensureData();

        window.displayTotals();

        window.currentChart = window.currentChart + 1;
        if (window.currentChart > 3) {
            window.currentChart = 0;
        }

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
                    fillColor: "rgba(220,220,220,0.5)",
                    strokeColor: "rgba(220,220,220,0.8)",
                    highlightFill: "rgba(220,220,220,0.75)",
                    highlightStroke: "rgba(220,220,220,1)",
                    data: []
                },
                {
                    label: "Kills",
                    fillColor: "rgba(151,187,205,0.5)",
                    strokeColor: "rgba(151,187,205,0.8)",
                    highlightFill: "rgba(151,187,205,0.75)",
                    highlightStroke: "rgba(151,187,205,1)",
                    data: []
                }
            ]
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

    window.displayTotals = function () {
        window.ensureData();
        $(".total_saves").text(window.stats.total.saves);
        $(".total_kills").text(window.stats.total.kills);
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
                        "#F7464A",
                        "#46BFBD",
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

