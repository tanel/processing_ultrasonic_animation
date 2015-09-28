/*global $:false, window:false, Chart:false, document:false */
"use strict";
$(function () {
    // Configuration
    window.settings = {
        ajaxIntervalMillis: 10000,
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

    // We'll rotate charts
    window.currentChart = 0;

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

        var data = {
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
            // Get the context of the canvas element we want to select
            ctx = document.getElementById("barchart").getContext("2d");

        $.each(window.stats.history, function (key, val) {
            data.labels.push(key);
            data.datasets[0].data.push(val.saves);
            data.datasets[1].data.push(val.kills);
        });

        window.myBar = new Chart(ctx, {
            type: 'bar',
            data: data,
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

