/*global $:false, window:false */
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
    };

    // update charts with given interval
    window.ajaxTimer = window.setInterval(window.chartFunc, window.settings.graphIntervalMillis);

    // start by drawing (empty) chart
    window.chartFunc();

    console.log("initialized");

    // Start by updating data
    window.ajaxFunc();
});

