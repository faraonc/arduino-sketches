// Smart Doorbell Javascript DOM Manipulation

$( document ).ready(function() {

	$('body').css("background-color", "white").css("color", "b7a57a"); //alt for bg-color black
	$('body').children().eq(1).css("padding", "0 5% 5% 5%");
	$('#team').text("Optimistic");
	$('#jumbox').css("background-color", "#4b2e83").css("color", "#fff");
	$('#db-header').text("Smart Doorbell Dashboard");
	$('hr').css("border-top", "1px solid #85754d");
	$('#foot').text("\u00a9 2017 Copyright: Pouria & Conard");
	$('.col-sm-4:nth-child(1)').css("color", "black"); //alt #a6a6a6
	$('.col-sm-4:nth-child(2)').css("color", "black");
	$('.col-sm-4:nth-child(3)').css("color", "black");

	// Messages for the user based on the weather
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
	broadcasts[advices.CLOUDY_SUNNY] = new Broadcast("Behind clouds the sun is shining.", "Get sunshine on a cloudy day.");
	broadcasts[advices.SUNNY] = new Broadcast("Sun came out to play.", "Don't forget your sunscreen.");
	broadcasts[advices.NIGHT] = new Broadcast("It is just dark out there.", "Turn your lights on, please.");
	broadcasts[advices.SNOW] = new Broadcast("Those snowy days.", "Stay warm!");
	broadcasts[advices.RAIN] = new Broadcast("Rain, rain go away.", "Bring your raincoat.");
	broadcasts[advices.DEFAULT] = new Broadcast("Monkeys are working.", "Don't disturb the monkeys.");

	// Checks the weather condition outside and loads the appropriate picture
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

		}else if(($('#light-value').text() === "DARK" || $('#light-value').text() === "DIM") && $('#rain-value').text() === "DRY"){
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

	$.fn.checkAirQuality = function() {
		var co = parseInt($('#co-value').text());
		var co2 = parseInt($('#co2-value').text());
		var dust = parseFloat($('#dust-value').text());
		var lpg = parseInt($('#lpg-value').text());
		var smoke = parseInt($('#smoke-value').text());

		// determine if the air quality is safe
		if(co >= 50 || co2 >= 3000 || dust >= 0.5 || lpg >= 1000 || smoke >= 400)
		{
			this.css("color", "red").text("UNSAFE");
		}else{
			this.css("color", "green").text("SAFE");
		}
	};

	$.fn.checkLight = function(){
		switch(this.text()){
			case "DARK":{
				this.addClass("alert alert-danger");
				break;
			}
			case "DIM":{
				this.addClass("alert alert-info");
				break;
			}
			case "SEMI BRIGHT":{
				this.addClass("alert alert-success");
				break;
			}
			case "BRIGHT":{
				this.addClass("alert alert-warning");
				break;
			}
			case "VERY BRIGHT":{
				this.addClass("alert alert-danger");
				
			}
		}
	}

	$.fn.checkRain = function(){
		switch(this.text()){
			case "HEAVY":{
				this.addClass("alert alert-danger");
				break;
			}
			case "WET":{
				this.addClass("alert alert-warning");
				break;
			}
			case "DRY":{
				this.addClass("alert alert-info");
			}
		}
	}

	$.fn.checkTemp = function() {
		var temp = parseInt(this.text());

		if(temp <= 25){
			this.addClass("alert alert-danger");

		} else if(temp >= 90){
			this.addClass("alert alert-danger");
		}
		else if(temp >= 70){
			this.addClass("alert alert-warning");

		}else if(temp >= 55){
			this.addClass("alert alert-success");

		}else{
			this.addClass("alert alert-info");
		}
	};

	$.fn.checkHumid = function() {
		var humid = parseInt(this.text());

		if(humid <= 5){
			this.addClass("alert alert-danger");

		} else if(humid >= 95){
			this.addClass("alert alert-warning");
		}
		else if(humid >= 50){
			this.addClass("alert alert-success");

		}else{
			this.addClass("alert alert-info");
		}
	};

	$.fn.checkDust = function() {
		var dust = parseFloat(this.text());

		if(dust >= 0.7){
			this.addClass("alert alert-danger");

		} else if(dust >= 0.4){
			this.addClass("alert alert-warning");
		}
		else if(dust >= 0.2){
			this.addClass("alert alert-success");

		}else{
			this.addClass("alert alert-info");
		}
	};

	$.fn.checkSmoke = function() {
		var smoke = parseInt(this.text());

		if(smoke >= 300){
			this.addClass("alert alert-danger");

		} else if(smoke >= 100){
			this.addClass("alert alert-warning");
		}
		else if(smoke >= 50){
			this.addClass("alert alert-success");

		}else{
			this.addClass("alert alert-info");
		}
	};

	$.fn.checkCO = function() {
		var co = parseInt(this.text());

		if(co >= 50){
			this.addClass("alert alert-danger");

		} else if(co >= 20){
			this.addClass("alert alert-warning");
		}
		else if(co >= 10){
			this.addClass("alert alert-success");

		}else{
			this.addClass("alert alert-info");
		}
	};

	$.fn.checkCO2 = function() {
		var co2 = parseInt(this.text());

		if(co2 >= 3000){
			this.addClass("alert alert-danger");

		} else if(co2 >= 2000){
			this.addClass("alert alert-warning");
		}
		else if(co2 >= 1000){
			this.addClass("alert alert-success");

		}else{
			this.addClass("alert alert-info");
		}
	};

	$.fn.checkLPG = function() {
		var lpg = parseInt(this.text());

		if(lpg >= 700){
			this.addClass("alert alert-danger");

		} else if(lpg >= 500){
			this.addClass("alert alert-warning");
		}
		else if(lpg >= 50){
			this.addClass("alert alert-success");

		}else{
			this.addClass("alert alert-info");
		}
	};

	$.fn.checkMotion = function() {
		switch(this.text()){
			case "Motion Detected":{
				this.css("background-color", "red");
				break;
			}
			case "No Motion":{
				this.css("background-color", "green");
			}
			
		}
	}

	$('#date-today').text(new Date().toString());
	$('#weather').addClass("text-left img-thumbnail text-center").css("padding", "20px");
	$('#weather h1').text("Today's Local Forecast");
	$('#weather-icon').addClass("rounded img-fluid").checkWeather();
	$('#weather .col-sm-6').css("color", "black");

	// update the time every sec
	setInterval(function(){$('#date-today').text(new Date().toString());}, 1000);

	$('#blink').attr("title", "Click to reset motion detection.").css("color", "white").checkMotion();

	// blinks the motion detection link
	function blinker() {
		$('#blink').fadeOut(500);
		$('#blink').fadeIn(500);
	}
	setInterval(blinker, 1500); //Runs every second

	$('#air-quality span').checkAirQuality();
	$('#light-value').checkLight();
	$('#rain-value').checkRain();
	$('#temp-value').checkTemp();
	$('#humid-value').checkHumid();
	$('#dust-value').checkDust();
	$('#smoke-value').checkSmoke();
	$('#co-value').checkCO();
	$('#co2-value').checkCO2();
	$('#lpg-value').checkLPG();

	function updateStates(){
		var request = new XMLHttpRequest();
		request.onreadystatechange = function(){
			if (this.readyState == 4 && this.status == 200) {
				if(this.responseText != null && this.responseText != ""){
					console.log(this.responseText);

					var json_arduino = JSON.parse(this.responseText)
					$('#light-value').text(json_arduino.light);
					$('#rain-value').text(json_arduino.rain);
					$('#temp-value').html(json_arduino.temp + "&#8457;");
					$('#humid-value').text(json_arduino.humid + "%");
					$('#dust-value').html(json_arduino.dust + " mg/m<sup>3</sup>");
					$('#smoke-value').text(json_arduino.smoke + " ppm");
					$('#co-value').text(json_arduino.co + " ppm");
					$('#co2-value').text(json_arduino.co2 + " ppm");
					$('#lpg-value').checkLPG(json_arduino.lpg + " ppm");

					$('.dropdown-menu li').children('a').eq(0).text("Master to Slave: " + json_arduino.master_slave);
					$('.dropdown-menu li').children('a').eq(1).text("Slave to Master: " + json_arduino.slave_master);
					$('.dropdown-menu li').children('a').eq(2).text("Master to Terminal: " + json_arduino.master_terminal);
					$('.dropdown-menu li').children('a').eq(3).text("Terminal to Master: " + json_arduino.terminal_master);

					$('#light-value').removeClass();
					$('#rain-value').removeClass();
					$('#temp-value').removeClass();
					$('#humid-value').removeClass();
					$('#dust-value').removeClass();
					$('#smoke-value').removeClass();
					$('#co-value').removeClass();
					$('#co2-value').removeClass();
					$('#lpg-value').removeClass();

					$('#blink').text(json_arduino.motion);
					$('#air-quality span').checkAirQuality();
					$('#light-value').checkLight();
					$('#rain-value').checkRain();
					$('#temp-value').checkTemp();
					$('#humid-value').checkHumid();
					$('#dust-value').checkDust();
					$('#smoke-value').checkSmoke();
					$('#co-value').checkCO();
					$('#co2-value').checkCO2();
					$('#lpg-value').checkLPG();
					$('#blink').checkMotion();
				}

			}
		}
		request.open("GET", "ajax", true);
		request.send(null);
	}

	setInterval(updateStates, 60000);

});












