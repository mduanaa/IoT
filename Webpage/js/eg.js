(function($) {

	$.ajaxSetup({
    cache: false
	});

	//plot format
	var layout = {

		// title: 'Ouput - CYT3014 (eduroam)',
		showlegend: false,

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
				// title: 'Time Gap (10s)',
	            type: "date",
	            tickformat: "%H:%M:%S",
	            showline: true,
	            showgrid: true,
	            zeroline: false,
	            mirror: true,
	        },

	    yaxis: {
	    		// title: 'TouchSensor Output',
	            showline: true,
	            showgrid: true,
	            zeroline: false,
	            mirror: true,
	            // range: [0, 120],
	        },

	    // legend: {
	    //         x: 0.8,
	    //         bgcolor: "rgba(255,255,255,0)",
	    //     },
	};

	var interVal;
	var interVal2;
	var graphics;
	var maxpoint = 360; //last one hour 360

	var trace = {
	  // type: 'scatter',
	  x: [],
	  y: [],

	  line: {
    			color: 'rgb(250,128,114)',
    			width: 3
  			}
	};


	//******** image size function ********//
	function get_imageSize(element){
		var width = $("#"+element).width()*0.95;
		var height = width/1.45;
		return {width: width, height: height};
	}

	//******** video load function ********//
	// function video_load(){

	// 	clearInterval(interVal);
	// 	var imagesize = get_imageSize(graphics[0].id);
	// 	var video = $("#"+"demo");
	// 	video.height = imagesize['height'];
	// 	video.width = imagesize['width'];
	// }

	//******** plot function ********//
	function plot_realTimedata(){
		//********show figure once open the webpage********//
		clearInterval(interVal); 
		
			//******** touch sensor ********//
		$.getJSON('touchpad.json', function(sdata){
		// $.getJSON('/lib/eg.py', function(sdata){
			trace['x'] = sdata['time']
			trace['y'] = sdata['touch'];
			var numpoint = sdata['touch'].length;
			if (numpoint > maxpoint) {
				trace['x'].splice(0, numpoint-maxpoint);
				trace['y'].splice(0, numpoint-maxpoint); 
			}
			// trace['x'] = Array(trace['y'].length).fill().map((x,i)=>i);
			var imagesize = get_imageSize(graphics[1].id);
			// layout['title'] = graphics[0].id + ' Ouput of the Last 1 Hour - CYT3014 (eduroam)';
			layout['height'] = imagesize['height'];
			layout['width'] = imagesize['width'];
			Plotly.plot('touchSensor', [trace], layout);
		});
	// }

		//********update figure every 30s********//
		interVal = setInterval(function(){
		//clear existing figure
		// Plotly.purge('touchSensor');
		//plot new figure
		// $.getJSON('touchpad.json?' + new Date().getTime(), function(sdata){
			//******** touch sensor ********//
		$.getJSON('touchpad.json', function(sdata){
			trace['x'] = sdata['time']
			trace['y'] = sdata['touch'];
			var numpoint = sdata['touch'].length;
			if (numpoint > maxpoint) {
				trace['x'].splice(0, numpoint-maxpoint);
				trace['y'].splice(0, numpoint-maxpoint);  
			}
			// trace['x'] = Array(trace['y'].length).fill().map((x,i)=>i);
			var imagesize = get_imageSize(graphics[1].id);
			// layout['title'] = graphics[0].id + ' Ouput of the Last 1 Hour - CYT3014 (eduroam)';
			layout['height'] = imagesize['height'];
			layout['width'] = imagesize['width'];
			Plotly.extendTraces('touchSensor', {y: [trace['y']],}, [0]);
			// console.log(sdata.length);
			// console.log(trace['x']);
			});
		}, 30000);
	}


	//********TBA replaced as local time********//
	interVal2 = setInterval(myTimer, 1000);
	function myTimer() {
	  var d = new Date();
	  var t = d.toLocaleTimeString();
	  $("#pH").html('Local Time: ' + t); 
	  $("#temperature").html('Local Time: ' + t);
	  $("#velocity").html('Local Time: ' + t);
	  $("#lightness").html('Local Time: ' + t);
	  //debug
	  // $.getJSON('/lib/eg.py', function(sensor){
	  	// console.log('sensor')
	  // });
	}


	// get graphics after page ready
	$(document).ready(function() { 
        graphics = $('.graphic');
    	plot_realTimedata();

		// callback function
		// function myStopFunction() {
		//   clearInterval(myVar);
		// }

		//self-invoking function
		// (function myStopFunction() {
		//   clearInterval(myVar);
		// })()

	    // Resize on window resize.
	    $(window).resize(function() {
	        // for (var i = 0; i < graphics.length; i++) {
	            var elem = graphics[0].id;
	            if ($("#"+elem+" > div").length > 0) {
	                Plotly.relayout(elem,get_imageSize(elem));
	            }
	        // }
	    });

    });

})(jQuery);
