$( document ).ready(function() {

	function Broadcast(status, advice){
		this.status = status;
		this.advice = advice;
	}

	var advices = {
		CLOUDY: 0,
		CLOUDY_SUNNY: 1,
		SUNNY: 2,
		NIGHT: 3,
		SNOW: 4,
		RAIN: 5,
		DEFAULT: 6
	}

	var broadcasts = [];
	broadcasts[advices.CLOUDY] = new Broadcast("The sun is hiding behind clouds.", "Smile on a cloudy day.");
	broadcasts[advices.CLOUDY_SUNNY] = new Broadcast("Behind the clouds the sun is shining.", "Get sunshine on a cloudy day.");
	broadcasts[advices.SUNNY] = new Broadcast("Sun came out to play.", "Don't forget your sunscreen.");
	broadcasts[advices.NIGHT] = new Broadcast("It is just dark out there.", "Turn your lights on, please.");
	broadcasts[advices.SNOW] = new Broadcast("Those snowy days.", "Stay warm!");
	broadcasts[advices.RAIN] = new Broadcast("Rain, rain go away.", "Bring your raincoat.");
	broadcasts[advices.DEFAULT] = new Broadcast("Monkeys are working.", "Don't disturb the monkeys.");


	$.fn.checkWeather = function() {

		var temp = parseInt($('#temp-value').text());

		if($('#light-value').text() === "SEMI BRIGHT" && $('#rain-value').text() === "DRY"){
			this.attr("src", "http://students.washington.edu/faraonc/Arduino/webpage/images/cloudy.svg");
			$('#status-day').text(broadcasts[advices.CLOUDY].status);
			$('#advice-day').text(broadcasts[advices.CLOUDY].advice);

		}else if($('#light-value').text() === "BRIGHT" && $('#rain-value').text() === "DRY"){
			this.attr("src", "http://students.washington.edu/faraonc/Arduino/webpage/images/cloudysunny.svg")
			$('#status-day').text(broadcasts[advices.CLOUDY_SUNNY].status)
			$('#advice-day').text(broadcasts[advices.CLOUDY_SUNNY].advice)

		}else if($('#light-value').text() === "VERY BRIGHT" && $('#rain-value').text() === "DRY"){
			this.attr("src", "http://students.washington.edu/faraonc/Arduino/webpage/images/sunny.svg");
			$('#status-day').text(broadcasts[advices.SUNNY].status);
			$('#advice-day').text(broadcasts[advices.SUNNY].advice);

		}else if($('#light-value').text() === "DARK" && $('#rain-value').text() === "DRY"){
			this.attr("src", "http://students.washington.edu/faraonc/Arduino/webpage/images/night.svg");
			$('#status-day').text(broadcasts[advices.NIGHT].status);
			$('#advice-day').text(broadcasts[advices.NIGHT].advice);

		}else if(($('#rain-value').text() === "WET" || $('#rain-value').text() === "HEAVY") && temp <= 32){
			this.attr("src", "http://students.washington.edu/faraonc/Arduino/webpage/images/snowing.svg");
			$('#status-day').text(broadcasts[advices.SNOW].status);
			$('#advice-day').text(broadcasts[advices.SNOW].advice);

		}else if(($('#rain-value').text() === "HEAVY" || $('#rain-value').text() === "WET") && temp > 32){
			this.attr("src", "http://students.washington.edu/faraonc/Arduino/webpage/images/raining.svg");
			$('#status-day').text(broadcasts[advices.RAIN].status);
			$('#advice-day').text(broadcasts[advices.RAIN].advice);

		}else{
			this.attr("src", "http://students.washington.edu/faraonc/Arduino/webpage/images/misc.svg");
			$('#status-day').text(broadcasts[advices.DEFAULT].status);
			$('#advice-day').text(broadcasts[advices.DEFAULT].advice);
		}

	};

	$.fn.checkAir = function() {
		var co = parseInt($('#co-value').text());
		var co2 = parseInt($('#co2-value').text());
		var dust = parseFloat($('#dust-value').text());
		var lpg = parseInt($('#lpg-value').text());
		var smoke = parseInt($('#smoke-value').text());

		if(co >= 50 || co2 >= 3000 || dust >= 0.5 || lpg >= 1000 || smoke >= 400)
		{
			this.css("color", "red").text("UNSAFE");
		}else{
			this.css("color", "green").text("SAFE");
		}


	};

	$('#date-today').text(new Date().toString());
	$('#weather').addClass("text-left img-thumbnail text-center").css("padding", "20px");
	$('#weather h1').text("Today's Local Forecast");
	$('#weather-icon').addClass("rounded img-fluid").checkWeather();
	$('#weather .col-sm-6').css("color", "black");
	setInterval(function(){$('#date-today').text(new Date().toString());}, 1000);
	$('#air-quality span').checkAir();



});