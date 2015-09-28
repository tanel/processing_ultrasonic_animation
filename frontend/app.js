/*global $:false, window:false, Chart:false */
"use strict";
$(function () {
    // Configuration
    window.settings = {
        ajaxIntervalMillis: 5000,
        graphIntervalMillis: 2000,
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
            window.chartFunc();
        });
    };

    // update data with a given interval
    window.ajaxTimer = window.setInterval(window.ajaxFunc, window.settings.ajaxIntervalMillis);

    // We'll rotate charts
    window.currentChart = 0;

    // callback func for drawing charts
    window.chartFunc = function () {
        console.log("updating charts");
        // FIXME: redraw chart
        // FIXME: rotate chart

        $(".total_saves").text(window.stats.totalSaves);
        $(".total_kills").text(window.stats.totalKills);

        var data = {
            labels: ["January", "February", "March", "April", "May", "June", "July"],
            datasets: [
                {
                    label: "My First dataset",
                    fillColor: "rgba(220,220,220,0.5)",
                    strokeColor: "rgba(220,220,220,0.8)",
                    highlightFill: "rgba(220,220,220,0.75)",
                    highlightStroke: "rgba(220,220,220,1)",
                    data: [65, 59, 80, 81, 56, 55, 40]
                },
                {
                    label: "My Second dataset",
                    fillColor: "rgba(151,187,205,0.5)",
                    strokeColor: "rgba(151,187,205,0.8)",
                    highlightFill: "rgba(151,187,205,0.75)",
                    highlightStroke: "rgba(151,187,205,1)",
                    data: [28, 48, 40, 19, 86, 27, 90]
                }
            ]
        };
        var options = {};

        // Get the context of the canvas element we want to select
        var ctx = document.getElementById("barchart").getContext("2d");
        window.myBar = new Chart(ctx, {
                type: 'bar',
                data: data,
                options: {
                    responsive: true,
                }
            });        
    };

    // update charts with given interval
    window.ajaxTimer = window.setInterval(window.chartFunc, window.settings.graphIntervalMillis);

    // start by drawing (empty) chart
    window.chartFunc();

    console.log("initialized");

    // Start by updating data
    window.ajaxFunc();
});

