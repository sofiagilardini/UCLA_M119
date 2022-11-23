// based on the example on https://www.npmjs.com/package/@abandonware/noble

const noble = require('@abandonware/noble');

const uuid_service = "1101"
const uuid_value = "2101"
const uuid_value2 = "2102"
const uuid_value3 = "2103"
const uuid_value4 = "2104"
const uuid_value5 = "2105"
const uuid_value6 = "2106"



let sensorValue = NaN
let sensorValue2 = NaN
let sensorValue3 = NaN
let sensorValue4 = NaN
let sensorValue5 = NaN
let sensorValue6 = NaN

// let sensorValue2 = NaN



noble.on('stateChange', async (state) => {
    if (state === 'poweredOn') {
        console.log("start scanning")
        await noble.startScanningAsync([uuid_service], false);
    }
});

noble.on('discover', async (peripheral) => {
    await noble.stopScanningAsync();
    await peripheral.connectAsync();
    const {
        characteristics
    } = await peripheral.discoverSomeServicesAndCharacteristicsAsync([uuid_service], [uuid_value, uuid_value2, uuid_value3, uuid_value4, uuid_value5, uuid_value6]);
    readData(characteristics[0], characteristics[1], characteristics[2], characteristics[3], characteristics[4], characteristics[5] )
});

//
// read data periodically
//
let readData = async (characteristic, characteristic2, characteristic3, characteristic4, characteristic5, characteristic6) => {
    const value = (await characteristic.readAsync());
    const value2 = (await characteristic2.readAsync());
    const value3 = (await characteristic3.readAsync());
    const value4 = (await characteristic4.readAsync());
    const value5 = (await characteristic5.readAsync());
    const value6 = (await characteristic6.readAsync());

    sensorValue = value.readFloatLE(0);
    sensorValue2 = value2.readFloatLE(0);
    sensorValue3 = value3.readFloatLE(0);
    sensorValue4 = value4.readFloatLE(0);
    sensorValue5 = value5.readFloatLE(0);
    sensorValue6 = value6.readFloatLE(0);


    console.log(sensorValue, sensorValue2, sensorValue3, sensorValue4, sensorValue5, sensorValue6);


    // read data again in t milliseconds
    setTimeout(() => {
        // readData(characteristic)
       readData(characteristic, characteristic2, characteristic3, characteristic4, characteristic5, characteristic6)
    }, 10);
}

//
// hosting a web-based front-end and respond requests with sensor data
// based on example code on https://expressjs.com/
//
const express = require('express')
const app = express()
const port = 3000

app.set('view engine', 'ejs');

app.get('/', (req, res) => {
    res.render('pong_2player')
})

app.post('/', (req, res) => {
    res.writeHead(200, {
        'Content-Type': 'application/json'
    });
    res.end(JSON.stringify({
        sensorValue2: sensorValue2
    }))
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
    
})