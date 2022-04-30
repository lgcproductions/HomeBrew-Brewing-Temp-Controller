
//---------------------------------------------------------
const char MAIN_PAGE[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title> Fermentation Heating/Cooling Controller </title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
            body {
                color: #434343;
                font-family: "Helvetica Neue",Helvetica,Arial,sans-serif;
                font-size: 14px;
                line-height: 1.42857142857143;
                padding: 20px;
            }

            .container {
                margin: 0 auto;
                max-width: 400px;
            }

            form .field-group {
                box-sizing: border-box;
                clear: both;
                padding: 4px 0;
                position: relative;
                margin: 1px 0;
                width: 100%;
            }

            form .field-group > label {
                color: #757575;
                display: block;
                margin: 0 0 5px 0;
                padding: 5px 0 0;
                position: relative;
                word-wrap: break-word;
            }

            input[type=button], input[type=submit], input[type=reset] {
                box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24),0 17px 50px 0 rgba(0,0,0,0.19);
                background-color: #e7e7e7; /* Green */
                color: black;
                border-radius: 12px;
                border: none;
                padding: 4px 5px;
                text-decoration: none;
                margin: 4px 2px;
                cursor: pointer;
            }

            input[type=text]:focus {
                border-color: #4C669F;
                outline: 0;
            }

            .button {
                background-color: #4CAF50; /* GREY */
                border-radius: 12px;
                border: none;
                color: white;
                padding: 9px 22px;
                text-align: center;
                text-decoration: none;
                display: inline-block;
                font-size: 16px;
                cursor: pointer;
            }
            .button2 {
                background-color: #008CBA;
                box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24),0 17px 50px 0 rgba(0,0,0,0.19);
            } /* Blue */
            .button3 {
                background-color: #f44336;
                box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24),0 17px 50px 0 rgba(0,0,0,0.19);
            } /* Red */ 
            .button4 {
                background-color: #e7e7e7; color: black;
                box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24),0 17px 50px 0 rgba(0,0,0,0.19);
            } /* Gray */ 
            .button5 {
                background-color: #555555;
                box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24),0 17px 50px 0 rgba(0,0,0,0.19);
            } /* Black */
            
            .button-container {
                box-sizing: border-box;
                clear: both;
                margin: 1px 0 0;
                padding: 4px 0;
                position: relative;
                width: 100%;
            }

            button[type=submit] {
                box-sizing: border-box;
                background: #f5f5f5;
                border: 1px solid #bdbdbd;
                border-radius: 2px;
                color: #434343;
                cursor: pointer;
                display: inline-block;
                font-family: inherit;
                font-size: 14px;
                font-variant: normal;
                font-weight: 400;
                height: 2.14285714em;
                line-height: 1.42857143;
                margin: 0;
                padding: 4px 10px;
                text-decoration: none;
                vertical-align: baseline;
                white-space: nowrap;
            }
        </style>
    </head>
<body>

<div class="container">
        <h1 style="text-align: center;">Fermentation Heating/Cooling Controller</h1>
      </div>
<div class="container">
  <table>
<tbody>
<tr>
<td>Temp Currently Is : <span id="temperature">0</span>&deg;C</td>
<td>Trigger Temp set to : <span id="trigTemperature">0</span>&deg;C +- <span id="TempDelay">0</span>&deg;C</td>
</tr>
<tr>
<td>Heating is : <span id="HEATINGState">NA</span></td>
<td>Cooling is : <span id="COOLINGState">NA</span></td>
</tr>
</tbody>
</table>
</div>
<br><br>
<div class="container">   
  <button type="button" class="button button3" onclick="sendData(1)">HEATING ON</button>
  <button type="button" class="button button3" onclick="sendData(0)">HEATING OFF</button><br><br>
  <button type="button" class="button button2" onclick="sendDataTwo(1)">COOLING ON</button>
  <button type="button" class="button button2" onclick="sendDataTwo(0)">COOLING OFF</button><br>
  <br>
  <form action="/action_page">
  <input type="text" name="trigtempdata" value="TRIG TEMP">
  <input type="submit" value="Submit">
  </form>
  <br>
  <form action="/temp_delay">
  <input type="text" name="tempdelaydata" value="Delay +/-">
  <input type="submit" value="Submit">
  </form>
  
</div>
<script>
function sendData(heating) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("HEATINGState").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "setHEATING?HEATINGstate="+heating, true);
  xhttp.send();
}

function sendDataTwo(cooling) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("COOLINGState").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "setCOOLING?COOLINGstate="+cooling, true);
  xhttp.send();
}

setInterval(function() {
  // Call a function repetatively with 2 Second interval
  getTempData();
  getTrigData();
  getTempDelay();
  getHeatingState();
  getCoolingState();
}, 2000); //2000mSeconds update rate


function getTempData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readTEMP", true);
  xhttp.send();
}

function getTrigData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("trigTemperature").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readTRIGTEMP", true);
  xhttp.send();
}

function getTempDelay() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("TempDelay").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readGETTEMPDELAY", true);
  xhttp.send();
}

function getHeatingState() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("HEATINGState").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "getHEATING", true);
  xhttp.send();
}

function getCoolingState() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("COOLINGState").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "getCOOLING", true);
  xhttp.send();
}

</script>
<br><br>
</body>
</html>
)=====";
