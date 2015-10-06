/*global $:false, window:false, Highcharts:false, document:false, moment:false, Chart:false */
"use strict";
$(function () {
    // Configuration
    window.settings = {
        ajaxIntervalMillis: 2000,
        chartRotateIntervalMillis: 5000,
        dateFormat: "DD.MM.YYYY",
        jsonDateFormat: "YYYY-MM-DD",
        killColor: "#000000",
        saveColor: "#FAFAFA",
        chartOptions: {
            responsive: false,
        },
    };

    // we'll keep data here
    window.stats = {};

    // callback func for ajax timer
    window.ajaxFunc = function () {
        console.log("updating data using AJAX");
        $.get("http://localhost:8000/gamestats.json", function (data) {
            console.log(data);
            window.stats = data;
        }).always(function () {
            window.setTimeout(window.ajaxFunc, window.settings.ajaxIntervalMillis);
        });
    };

    Highcharts.setOptions({
        colors: [window.settings.saveColor, window.settings.killColor]
    });

    // the chart currently being displayed,
    // since we start with empty page, the value is -1.
    window.currentChart = -1;

    // callback func chart rotating
    window.chartRotateFunc = function () {
        console.log("rotating charts");

        window.ensureData();

        window.displayDates();

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
        // (the indices respond to chart ID's in HTML)
        if (3 === window.currentChart) {
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
        var categories = [], saves = [], kills = [];
        $.each(window.stats.history, function (key, val) {
            categories.push(key);
            saves.push(val.saves);
            kills.push(val.kills);
        });

        $('#barchart').highcharts({
            chart: {
                type: 'column'
            },
            title: {
                text: ''
            },
            xAxis: {
                categories: categories,
                crosshair: true
            },
            yAxis: {
                min: 0,
                title: {
                    text: ''
                }
            },
            tooltip: {
                headerFormat: '<span style="font-size:10px">{point.key}</span><table>',
                pointFormat: '<tr><td style="color:{series.color};padding:0">{series.name}: </td>' +
                    '<td style="padding:0"><b>{point.y:.1f} mm</b></td></tr>',
                footerFormat: '</table>',
                shared: true,
                useHTML: true
            },
            plotOptions: {
                column: {
                    pointPadding: 0.2,
                    borderWidth: 0
                }
            },
            series: [{
                name: 'Saves',
                data: saves,
            }, {
                name: 'Kills',
                data: kills

            }]
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

        $('#piechart').highcharts({
            credits: false,
            chart: {
                type: 'pie',
                options3d: {
                    enabled: true,
                    alpha: 45,
                    beta: 0
                }
            },
            title: {
                text: ''
            },
            tooltip: {
                pointFormat: '{series.name}: <b>{point.percentage:.1f}%</b>'
            },
            plotOptions: {
                pie: {
                    allowPointSelect: true,
                    cursor: 'pointer',
                    depth: 35,
                    dataLabels: {
                        enabled: true,
                        format: '{point.name}'
                    },
                }
            },
            series: [{
                type: 'pie',
                name: 'HUMANITY vs CURIOSITY',
                data: [
                    ["HUMAANSUS / HUMANITY", window.stats.total.saves],
                    ["UUDISHIMU / CURIOSITY", window.stats.total.kills]
                ],
            }]
        });
    };

    console.log("initialized");

    window.chartRotateFunc();

    // Start by updating data
    window.ajaxFunc();
});

