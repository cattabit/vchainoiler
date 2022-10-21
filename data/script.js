// Get current sensor readings when the page loads
//window.addEventListener('load', getReadings);
//window.addEventListener('load', getSettings);

// Функция обновления значения элемента, где key = ID элемента, а Value - его значение.
function updateElementsValuesById(objData) {
	//console.log("updateElementsValuesById() ");
	for (var obj in objData) {
		if (obj == "MODE") {
			//console.log("MODE !!!!!!");
			document.getElementById("_MODE_SWITCH_1").className = "pure-button pure-button-primary";
			document.getElementById("_MODE_SWITCH_2").className = "pure-button pure-button-primary";
			document.getElementById("_MODE_SWITCH_3").className = "pure-button pure-button-primary";
			document.getElementById("_MODE_SWITCH_4").className = "pure-button pure-button-primary";
			document.getElementById("_MODE_SWITCH_" + objData[obj]).className = "button-active pure-button";
		}

		if (obj == "MODE_MILAGE") {
			document.getElementById("_MODE_MILAGE_SWITCH_10").className = "pure-button pure-button-primary";
			document.getElementById("_MODE_MILAGE_SWITCH_20").className = "pure-button pure-button-primary";
			document.getElementById("_MODE_MILAGE_SWITCH_30").className = "pure-button pure-button-primary";
			document.getElementById("_MODE_MILAGE_SWITCH_" + objData[obj]).className = "button-active pure-button";
		}
		if (obj == "MODE_TIMER") {
			document.getElementById("_MODE_TIMER_SWITCH_10").className = "pure-button pure-button-primary";
			document.getElementById("_MODE_TIMER_SWITCH_20").className = "pure-button pure-button-primary";
			document.getElementById("_MODE_TIMER_SWITCH_30").className = "pure-button pure-button-primary";
			document.getElementById("_MODE_TIMER_SWITCH_40").className = "pure-button pure-button-primary";
			document.getElementById("_MODE_TIMER_SWITCH_" + objData[obj]).className = "button-active pure-button";
		}
		if (obj == "gps_fixmode") {
			if (document.getElementById("_gps_fixmode_btn")) {
				if (objData[obj] == "2") {
					document.getElementById("_gps_fixmode_btn").className = "button-active pure-button";
					document.getElementById("_gps_fixmode_btn").innerText = "2D";
				}
				else if (objData[obj] == "3") {
					document.getElementById("_gps_fixmode_btn").className = "button-active pure-button";
					document.getElementById("_gps_fixmode_btn").innerText = "3D";
				}
				else if (objData[obj] == "1") {
					document.getElementById("_gps_fixmode_btn").className = "pure-button pure-button-primary";
					document.getElementById("_gps_fixmode_btn").innerText = "No Fix";
				}
				else {
					document.getElementById("_gps_fixmode_btn").className = "pure-button pure-button-primary";
					document.getElementById("_gps_fixmode_btn").innerText = "NA";
				}
			}
		}


		//else { // остальные пытаемся установить по ID
		//console.log("_" + obj + " > " + objData[obj] + " (type: " + typeof objData[obj] + ")");
		if (typeof objData[obj] == 'string' || typeof objData[obj] == 'number') {
			try {


				if (document.getElementById("_" + obj)) {
					//console.log(document.getElementById("_" + obj));
					document.getElementById("_" + obj).value = objData[obj];
				} else {
					//console.log('updateElementsValuesById: Ошибка установки значения, не найден элемент id="_' + obj + '" (: ' + objData[obj] + '). Пропускаем.');
				}
			} catch (e) {
				console.log('updateElementsValuesById: Ошибка установки значения: _' + obj + " > " + objData[obj] + " | " + e.name + ":" + e.message + "\n" + e.stack);
				throw e;
			}
		}
		else {
			console.log("Not String or Number type of value! Skip!");
		}
	}
	//}
}

// Create Temperature Gauge
/*var gaugeTemp = new LinearGauge({
	renderTo: 'gauge-temperature',
	width: 120,
	height: 400,
	units: "Temperature C",
	minValue: 0,
	startAngle: 90,
	ticksAngle: 180,
	maxValue: 40,
	colorValueBoxRect: "#049faa",
	colorValueBoxRectEnd: "#049faa",
	colorValueBoxBackground: "#f1fbfc",
	valueDec: 2,
	valueInt: 2,
	majorTicks: [
		"0",
		"5",
		"10",
		"15",
		"20",
		"25",
		"30",
		"35",
		"40"
	],
	minorTicks: 4,
	strokeTicks: true,
	highlights: [{
			"from": 30,
			"to": 40,
			"color": "rgba(200, 50, 50, .75)"
		}
	],
	colorPlate: "#fff",
	colorBarProgress: "#CC2936",
	colorBarProgressEnd: "#049faa",
	borderShadowWidth: 0,
	borders: false,
	needleType: "arrow",
	needleWidth: 2,
	needleCircleSize: 7,
	needleCircleOuter: true,
	needleCircleInner: false,
	animationDuration: 1500,
	animationRule: "linear",
	barWidth: 10,
}).draw();

// Create Humidity Gauge
var gaugeHum = new RadialGauge({
	renderTo: 'gauge-humidity',
	width: 300,
	height: 300,
	units: "Humidity (%)",
	minValue: 0,
	maxValue: 100,
	colorValueBoxRect: "#049faa",
	colorValueBoxRectEnd: "#049faa",
	colorValueBoxBackground: "#f1fbfc",
	valueInt: 2,
	majorTicks: [
		"0",
		"20",
		"40",
		"60",
		"80",
		"100"

	],
	minorTicks: 4,
	strokeTicks: true,
	highlights: [{
			"from": 80,
			"to": 100,
			"color": "#03C0C1"
		}
	],
	colorPlate: "#fff",
	borderShadowWidth: 0,
	borders: false,
	needleType: "line",
	colorNeedle: "#007F80",
	colorNeedleEnd: "#007F80",
	needleWidth: 2,
	needleCircleSize: 3,
	colorNeedleCircleOuter: "#007F80",
	needleCircleOuter: true,
	needleCircleInner: false,
	animationDuration: 1500,
	animationRule: "linear"
}).draw();
*/


// Function to get current readings on the webpage when it loads for the first time
// Функция получения текущих значений при первой загрузке страницы
function getReadings() {
	var xhr = new XMLHttpRequest();
	// Запрос изменяемых параметров
	xhr.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			console.log("getReadings() Server response: " + this.responseText);
			var data = JSON.parse(this.responseText);
			console.log(data);
			updateElementsValuesById(data);
		}
	};
	xhr.open("GET", "/readings", true);
	xhr.send();
}

// Function to get current settings values on the webpage when it loads for the first time
// Функция получения текущих значений при первой загрузке страницы
function getSettings() {
	var xhr = new XMLHttpRequest();
	// Запрос значений настроек
	xhr.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			console.log("getSettings() Server response: " + this.responseText);
			var data = JSON.parse(this.responseText);
			console.log(data);
			updateElementsValuesById(data);
		}
	};
	xhr.open("GET", "/settings", true);
	xhr.send();
}

function refreshData() {
	getSettings();
	getReadings();
}

// Отправка значений медом POST в /post
function postSettingsReset() {
	console.log("postSettingsReset() started..");

	var xhr = new XMLHttpRequest();
	xhr.open("GET", '/settings_reset_req', true);
	xhr.send();

	xhr.onload = function() {
		console.log("this.responseText = " + this.responseText);
	};
}

// Отправка значений медом POST в /post
function postSettingsRequest(data) {
	console.log("postRequest() started..");

	var xhr = new XMLHttpRequest();
	xhr.open("POST", '/postSettings', true);
	xhr.send(data);

	xhr.onload = function() {
		console.log("this.responseText = " + this.responseText);
		getSettings(); // Обновим все значения настроек
	};
}

// Сохранение значений
function postSettingsMilage() {
	console.log("postSettingsMilage() started..");
	var data = new FormData();
	data.set('MIntervalTown', document.getElementById('_MIntervalTown').value);
	data.set('MIntervalRoad', document.getElementById('_MIntervalRoad').value);
	data.set('MRoadSpeed', document.getElementById('_MRoadSpeed').value);
	data.set('MMinSpeed', document.getElementById('_MMinSpeed').value)
	data.set('MCoefRain', document.getElementById('_MCoefRain').value)
	postSettingsRequest(data);
}

// Сохранение значений
function postSettingsTimer() {
	console.log("postSettingsTimer() started..");
	var data = new FormData();
	data.set('TIntervalTown', document.getElementById('_TIntervalTown').value);
	data.set('TIntervalRoad', document.getElementById('_TIntervalRoad').value);
	data.set('TIntervalRain', document.getElementById('_TIntervalRain').value);
	postSettingsRequest(data);
}

// Сохранение значений
function postSettingsGPS() {
	console.log("postSettingsGPS() started..");
	var data = new FormData();
	data.set('gpsMinCalcSpeed', document.getElementById('_gpsMinCalcSpeed').value);
	data.set('gpsAgeMax', document.getElementById('_gpsAgeMax').value);
	data.set('gpsUpdatePeriod', document.getElementById('_gpsUpdatePeriod').value);
	data.set('gpsDataWait', document.getElementById('_gpsDataWait').value);
	postSettingsRequest(data);
}


// Сохранение значений
function postSettingsPump() {
	console.log("postSettingsPump() started..");
	var data = new FormData();
	data.set('PUMP_count', document.getElementById('_PUMP_count').value);
	data.set('PNumberImp', document.getElementById('_PNumberImp').value);
	data.set('PImpInterval', document.getElementById('_PImpInterval').value);
	postSettingsRequest(data);
}

// Сохранение значений
function postSettingsWheel() {
	console.log("postSettingsWheel() started..");
	var data = new FormData();
	data.set('MWeelCircle', document.getElementById('_MWeelCircle').value);
	data.set('MSignalsOn1', document.getElementById('_MSignalsOn1').value);
	postSettingsRequest(data);
}

// Сохранение значений
function postSettingsOthers() {
	console.log("postSettingsOthers() started..");
	var data = new FormData();
	data.set('DurationImp', document.getElementById('_DurationImp').value);
	postSettingsRequest(data);
}



// Переключение основного режима
function postSetMode(value) {
	console.log("postSetMode(" + value + ") started..");
	var data = new FormData();
	data.set('MODE', value);
	postSettingsRequest(data);
}

// Переключение режима режима "По пробегу"

function postSetModeMilage(value) {
	console.log("postSetModeMilage(" + value + ") started..");
	var data = new FormData();
	data.set('MODE_MILAGE', value);
	postSettingsRequest(data);
}

// Переключение режима режима "По Времени"
function postSetModeTimer(value) {
	console.log("postSetModeTimer(" + value + ") started..");
	var data = new FormData();
	data.set('MODE_TIMER', value);
	postSettingsRequest(data);
}



if (!!window.EventSource) {
	var source = new EventSource('/events');
	source.addEventListener('open', function(e) {
		console.log("Events Connected");
	}, false);

	source.addEventListener('error', function(e) {
		if (e.target.readyState != EventSource.OPEN) {
			console.log("Events Disconnected");
		}
	}, false);

	source.addEventListener('message', function(e) {
		console.log("message", e.data);
	}, false);

	source.addEventListener('new_readings', function(e) {
		//console.log("new_readings", e.data);
		var data = JSON.parse(e.data);
		//console.log(data);
		updateElementsValuesById(data);

	}, false);

	source.addEventListener('new_settings', function(e) {
		console.log("new_settings", e.data);
		var data = JSON.parse(e.data);
		//console.log(data);
		updateElementsValuesById(data);

	}, false);
}
