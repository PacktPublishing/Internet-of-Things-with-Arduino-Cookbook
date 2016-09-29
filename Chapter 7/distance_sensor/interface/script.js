$( document ).ready(function() {

    // Device
    var address = "192.168.0.104";
    var device = new Device(address);

    // Buttons
    $('#forward').mousedown(function() {
      device.callFunction("forward");
    });
    $('#forward').mouseup(function() {
      device.callFunction("stop");
    });

    $('#right').mousedown(function() {
      device.callFunction("right");
    });
    $('#right').mouseup(function() {
      device.callFunction("stop");
    });

    $('#left').mousedown(function() {
      device.callFunction("left");
    });
    $('#left').mouseup(function() {
      device.callFunction("stop");
    });

    $('#backward').mousedown(function() {
      device.callFunction("backward");
    });
    $('#backward').mouseup(function() {
      device.callFunction("stop");
    });

    // Front distance sensor
    setInterval(function(){
      device.callFunction("distance", "", function(data) {
        $('#distance').text(data.return_value);
      });
    }, 500);

});
