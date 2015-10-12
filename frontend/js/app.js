/*global $:false, window:false, Highcharts:false, document:false, moment:false, Chart:false */
"use strict";
$(function () {
    // Configuration
    window.settings = {
        ajaxIntervalMillis: 2000,
        dateFormat: "DD.MM.YYYY",
        jsonDateFormat: "YYYY-MM-DD",
        chartDateFormat: "DD.MM",
        lastUpdateDateFormat: "DD.MM.YYYY HH:mm:ss",
        killColor: "#000000",
        saveColor: "#F2F2F2",
        borderColor: "#F9F9F9",
    };

    $('#toggle_last_update_panel').click(function () {
        $('#last_update_panel').toggle();
        return false;
    });

    // we'll keep data here
    window.stats = {};

    // need to update charts faster on initial load
    window.firstLoadDone = false;

    // callback func for ajax timer
    window.ajaxFunc = function () {
        console.log("updating data using AJAX");
        var url = "http://localhost:8000/gamestats.json";
        if ("ox.linnagalerii.ee" === window.location.host) {
            url = "http://ox.linnagalerii.ee/gamestats.json";
        }
        $.get(url, function (data) {
            console.log(data);
            window.stats = data;
            if (!window.firstLoadDone) {
                window.displayCharts();
                window.firstLoadDone = true;
            }
        }).fail(function (xhr, err) {
            $('#last_error').text(String(err) + " (" + String(xhr.status) + ")");
        }).success(function () {
            $('#last_error').text("ok");
        }).always(function () {
            $('#last_update').text(moment().format(window.settings.lastUpdateDateFormat));
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

        window.currentChart = window.currentChart + 1;
        if (window.currentChart > 3) {
            window.currentChart = 0;
        }

        window.displayCharts();
    };

    window.displayCharts = function () {
        window.ensureData();

        window.displayDates();

        window.displayTotals();


        // Hide visible chart(s)
        $('.stats').hide();

        // show the current chart
        var chart = $('#chart' + String(window.currentChart)),
            rotateIntervalSeconds = parseInt(chart.attr('data-interval-seconds'), 10);
        chart.show();

        // draw its internal contents.
        // (the indices respond to chart ID's in HTML)
        if (3 === window.currentChart) {
            window.displayBarChart();
        } else if (2 === window.currentChart) {
            window.displayPieChart();
        }

        // Rotate charts with N millis interval (see HTML)
        console.log("rotate interval seconds", rotateIntervalSeconds);
        if (rotateIntervalSeconds) {
            window.setTimeout(window.chartRotateFunc, rotateIntervalSeconds * 1000);
        } else {
            console.log("missing data-interval on chart! ");
        }
    };

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
            categories.push(moment(key).format(window.settings.chartDateFormat));
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
                    '<td style="padding:0"><b>{point.y:.1f}</b></td></tr>',
                footerFormat: '</table>',
                shared: true,
                useHTML: true
            },
            plotOptions: {
                column: {
                    pointPadding: 0.2,
                    borderColor: window.settings.borderColor,
                    borderRadius: 1,
                    borderWidth: 1,
                },
            },
            series: [{
                name: 'Säästetud elud / Saves',
                data: saves,
            }, {
                name: 'Hukkamised / Kills',
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
        if (!todaysResults) {
            todaysResults = {
                saves: 0,
                kills: 0,
            };
        }
        $(".today_saves").text(todaysResults.saves);
        $(".today_kills").text(todaysResults.kills);
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
                },
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

