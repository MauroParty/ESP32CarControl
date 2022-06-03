let Socket = new WebSocket('ws://' + window.location.hostname + ':81/');

let FnBSliderElement = document.getElementById("FwdnBckERangue");
let powerState = false;
let lastSend = 0;

let RPM = 288;          //REVOLUCIONES POR MINUTO
let mps = 1.12;         //METROS POR SEGUNDO

window.addEventListener('load',initWebSocket);

function initWebSocket(){
    Socket.onopen = function(event){
        console.log("WebSocket abierto.");
    };
    Socket.onclose = function(event){
        console.log("WebSocket cerrado.");
    };
    Socket.onerror = function(event){
        console.log("Error en WebSocket.");
    };
    Socket.onmessage = function(event){
        console.log(event);
    };
}

function power(){                                               //ESTA FUNCION CAMBIA EL ESTADO DEL POWERSTATE Y CAMBIA EL LED ENCENDIDO
    let element4 = document.getElementById("mainBox__dataBox-powerStatus-LED");
    powerState = !powerState;                   
    if(!powerState){
        Socket.send('D');
        element4.style.backgroundColor = "#ff0000";
    }else{
        element4.style.backgroundColor = "#00ff00";
    }
}

function clicking(direction){
    if(powerState){                                             //AQUI ESTA LA FUNCION DEL BOTON TOGGLE "POWER"
        switch(direction){
            case 'left':   Socket.send('L'); break;
            case 'right':  Socket.send('R'); break;
            case 'buzzer': Socket.send('S'); break;
        }
    }else{
        alert('No ha encendido el carro');              //EN CASO DE QUE NO SE HAYA ENCENDIDO EL CARRO.
    }
}

function sendFnBDirection(FnBSliderElement){
    if(!powerState){
        alert('No ha encendido el carro.');     //EN CASO DE QUE NO SE HAYA ENCENDIDO EL CARRO
        return;
    }
    let now = (new Date).getTime();
    if(lastSend > now-20) return;
    lastSend = now;

    let element1 = document.getElementById("mainBox__dataBox-tachometer-bar-progress");
    let element2 = document.getElementById("mainBox__dataBox-tachometer-RPM-value");
    let element3 = document.getElementById("mainBox__dataBox-tachometer-mps-value");
    let element4 = document.getElementById("rangeSlider");

    let revolution = 1;

    element4.style.bottom = 3+((FnBSliderElement)*3/32) + '%';

    console.log(FnBSliderElement);

    if(FnBSliderElement<=255){
        revolution = 255 - FnBSliderElement;
        Socket.send("B" + revolution);
    }else if(FnBSliderElement>=257){
        revolution = FnBSliderElement - 256;
        Socket.send("F" + revolution);
    }
    let speed = (revolution*100)/255;

    element1.style.width = speed + '%';
    element2.innerHTML = ((speed*RPM)/100).toFixed(1);     //AQUI SE HACE EL CALCULO PARA LAS REVOLUCIONES POR MINUTO
    element3.innerHTML = ((speed*mps)/100).toFixed(1);     //AQUI SE HACE EL CALCULO PARA LOS METROS POR SEGUNDO

    //NOTA: AQUI SE HACE EL CALCULO DE EL CARRO ESPECIFICO, 
    //SI CONSIGUEN UN DATASHEET DE LOS MOTORES CON LOS QUE VAN
    //A TRABAJAR, PUEDEN CAMBIAR LOS VALORES POR EL DE SU CARRO EN LA CABECERA.
}