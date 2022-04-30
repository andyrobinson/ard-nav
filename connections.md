# Connections

This is a list of all of the wiring connections to the Arduino Zero
<table>
  <tr>
    <td></td>
    <td></td>
    <td rowspan="18">
    <h2>&nbsp;&nbsp;Arduino Zero&nbsp;&nbsp;</h2>
    </td>
    <td>SCL</td>
    <td>Green - LSM303DLHC, AS5048B</td>
  </tr>
  <tr>
    <td></td>
    <td>ATN</td>
    <td>SDA</td>
    <td>Yellow - LSM303DLHC, AS5048B</td>
  </tr>
  <tr>
    <td></td>
    <td>IOREF</td>
    <td>AREF</td>
    <td></td>
  </tr>
  <tr>
    <td></td>
    <td>RESET</td>
    <td>GND</td>
    <td></td>
  </tr>
  <tr>
    <td>AS5048B, Brown(W)</td>
    <td>3.3v</td>
    <td>D13</td>
    <td>Brown - LSM303DLHC Vin</td>
  </tr>
  <tr>
    <td>Red - SD card, Servo controller, GPS</td>
    <td>5v</td>
    <td>D12</td>
    <td>Purple - Gps Tx</td>
  </tr>
  <tr>
    <td>Black(W)</td>
    <td>GND</td>
    <td>D11</td>
    <td>Red(W) - unassigned</td>
  </tr>
  <tr>
    <td></td>
    <td>GND</td>
    <td>D10</td>
    <td>Red - Gps Rx</td>
  </tr>
  <tr>
    <td></td>
    <td></td>
    <td>D9</td>
    <td>Yellow - Temp Switch bit 2</td>
  </tr>
  <tr>
    <td>Red - 7.5v from Solar</td>
    <td>VIN</td>
    <td>D8</td>
    <td>Blue - Temp Swtich bit 1</td>
  </tr>
  <tr>
    <td>White(W) - Solar Lipo v(1)</td>
    <td>A0</td>
    <td>D7</td>
    <td>Green - Temp Switch bit 0</td>
  </tr>
  <tr>
    <td>Yellow(W) - Solar Lipo v(2)</td>
    <td>A1</td>
    <td>D6</td>
    <td>Blue(W) - unassigned</td>
  </tr>
  <tr>
    <td></td>
    <td>A2</td>
    <td>D5</td>
    <td></td>
  </tr>
  <tr>
    <td></td>
    <td>A3</td>
    <td>D4</td>
    <td>Pink - SD Card CS</td>
  </tr>
  <tr>
    <td></td>
    <td>A4</td>
    <td>D3</td>
    <td>White - Servo controller Tx</td>
  </tr>
  <tr>
    <td>Yellow - Temp RC input</td>
    <td>A5</td>
    <td>D2</td>
    <td>Grey - Servo controller Rx</td>
  </tr>
  <tr>
    <td></td>
    <td></td>
    <td>D1 -> Tx</td>
    <td>Green(W) - Satellite Comms Rx (planned)</td>
  </tr>
  <tr>
    <td></td>
    <td></td>
    <td>D0 <- Rx</td>
    <td>Orange(W) - Satellite Comms Tx (planned)</td>
  </tr>

</table>
