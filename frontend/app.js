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

            // Force redraw of chart
            window.updateUI();
        });
    };

    // update data with a given interval
    window.ajaxTimer = window.setInterval(window.ajaxFunc, window.settings.ajaxIntervalMillis);

    // callback func chart rotating
    window.chartRotateFunc = function () {
        console.log("rotating charts");
        window.currentChart = window.currentChart + 1;
        if (window.currentChart > 3) {
            window.currentChart = 0;
        }
        // Hide visible chart(s)
        $('.stats').hide();
        $('#chart' + String(window.currentChart)).show();
    };

    // We'll rotate charts
    window.currentChart = 0;
    window.chartRotateTimer = window.setInterval(window.chartRotateFunc, window.settings.chartRotateIntervalMillis);

    window.updateUI = function () {
        console.log("updating charts");

        if (!window.stats.total) {
            window.stats.total = {};
        }

        $(".total_saves").text(window.stats.total.saves);
        $(".total_kills").text(window.stats.total.kills);

        if (!window.stats.history) {
            window.stats.history = {};
        }

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
            piedata = {
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
            barctx = document.getElementById("barchart").getContext("2d"),
            piectx = document.getElementById("piechart").getContext("2d");

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

        window.pieChart = new Chart(piectx, {
            type: 'pie',
            data: piedata,
            options: {
                responsive: false,
            }
        });
    };

    // draw an (empty) chart
    window.updateUI();

    console.log("initialized");

    // Start by updating data
    window.ajaxFunc();
});

