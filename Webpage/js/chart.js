(function($) {
    var layout = {
        font: {
            size: 12,
        },
        margin: {
            l: 40,
            r: 30,
            t: 30,
            b: 30,
        },
        xaxis: {
            type: "date",
            tickformat: "%H:%M:%S",
            showline: true,
            showgrid: true,
            zeroline: false,
            mirror: true,
        },
        yaxis: {
            showline: true,
            showgrid: true,
            zeroline: false,
            mirror: true,
        },
        legend: {
            x: 0.8,
            bgcolor: "rgba(255,255,255,0)",
        },
    };
    var graphics;
    var interval;

    //get element width and length
    function getImagesize(element) {
        var width = $("#"+element).width()*0.95; //$ alias for jQuery - javascript library. '#'+element: id+id_name 
        var height = width/1.45;      
        return {width: width, height: height}; // return image size in css way
    }

    //plot real-time data
    function plotRealtime(sensor) {       
        clearInterval(interval);                // stop timer loop function    
        var datas = Array(graphics.length).fill(); //create an 'None' 1-d array with specific length
        var stamp = Array(graphics.length).fill(); // create an 'None' 1-d array with specific length
        var maxpoint = 120;                        

        // Initial data.
        for (var i = 0; i < graphics.length; i++) { 
            Plotly.purge(graphics[i].id); //purge clean div and remove any existing plot
            (function(i) {
                stamp[i] = Date.now()-1000*60*60; // the past 1 hour
                // call python script and pass the value get from index.html, return value of the python passes into the following function
                $.getJSON('/lib/chart.py?action=latest_data&sensor='+sensor+'&type='+graphics[i].id+'&timestamp='+stamp[i], function(sdata) {
                    if (sdata['timestamp'] != null) {
                        stamp[i] = sdata['timestamp'];
                    }
                    datas[i] = [{
                        x: sdata['x'],
                        y: sdata['y'],
                    }];
                    var numpoint = datas[i][0].x.length; //number start point
                    if (numpoint > maxpoint) {
                        datas[i][0].x.splice(0, numpoint-maxpoint); //if number start point > 120, then start from 'number - 120'
                        datas[i][0].y.splice(0, numpoint-maxpoint);
                    }
                    var imagesize = getImagesize(graphics[i].id); // get image size
                    layout['width'] = imagesize['width']; 
                    layout['height'] = imagesize['height'];
                    Plotly.plot(graphics[i].id, datas[i], layout); //work on 'id'
                });
            })(i);
        }

        // Incoming data.
        // setInterval: execute function in every 30,000 miliseconds
        // update the dataplot in every 30seconds
        interval = setInterval(function() {
            for (var i = 0; i < graphics.length; i++) {(function(i) {
                $.getJSON('/lib/chart.py?action=latest_data&sensor='+sensor+'&type='+graphics[i].id+'&timestamp='+stamp[i], function(sdata) {
                    if (sdata['timestamp'] != null) {
                        stamp[i] = sdata['timestamp'];
                        var numpoint = datas[i][0].x.length;
                        if (numpoint > maxpoint) {
                            datas[i][0].x.splice(0, numpoint-maxpoint);
                            datas[i][0].y.splice(0, numpoint-maxpoint);
                        }
                        Plotly.extendTraces(graphics[i].id, {
                            x: [sdata['x']],
                            y: [sdata['y']],
                        }, [0]);
                    }
                });
            })(i);}
        }, 30000);
    }

    // function plotHistorical(sensor) {
    //     var type = $(".active a:first-child").attr("href").replace(/.*\/(.*)\.html/, '$1');
    //     for (var i = 0; i < graphics.length; i++) {
    //         (function(i) {
    //             var image = "/dataplots/"+sensor+"-"+type+"-"+graphics[i].id+".png";
    //             $.get(image)
    //             .done(function() {
    //                 $("#"+graphics[i].id).empty();
    //                 $("#"+graphics[i].id).append("<img src='"+image+"'/>");
    //             })
    //         })(i);
    //     }
    // }

    function plotData(sensor) {
        if ($(".graphic_realtime").length > 0) { //jquery check class = graphic_realtime
            plotRealtime(sensor);
        } else {
            //debug - error then show timer

            
        }
    }


    //$(document).ready(function()) - when document is loaded, function is executed. 
    $(document).ready(function() { 
        graphics = $(".graphic");

        // Fill dropdown and plot.
        // var choices = $("#sensor-selection");
        // if (choices.length > 0) {
        //     $.getJSON("/lib/chart.py?action=active_sensors", function(sensors) {
        //         choices.empty();
        //         if (!$.isEmptyObject(sensors)) {
        //             for (var index in sensors) {
        //                 choices.append("<option>"+sensors[index]+"</option>");
        //             }
        //             plotData($("#sensor-selection option:selected").text());
        //         } else {
        //             $(".alert-danger").show();
        //             $(".alert-danger span.message").text("No active sensors avaliable.");
        //         }
        //     });
        // }

        // Replot on sensor selection change.
        // $("#sensor-selection").change(function() {
        //     plotData($("#sensor-selection option:selected").text());
        // });

        // Resize on window resize.
        // $(window).resize(function() {
        //     for (var i = 0; i < graphics.length; i++) {
        //         var elem = graphics[i].id;
        //         if ($("#"+elem+" > div").length > 0) {
        //             Plotly.relayout(elem, getImagesize(elem));
        //         }
        //     }
        // });
    });
})(jQuery);
