var updateWiFiStatusInProgress = false;
var updateBoilerStatusInProgress = false;

function toHHMMSS(time) {
    var sec_num = parseInt(time, 10);
    var hours   = Math.floor(sec_num / 3600);
    var minutes = Math.floor((sec_num - (hours * 3600)) / 60);
    var seconds = sec_num - (hours * 3600) - (minutes * 60);

    if (hours   < 10) {hours   = "0"+hours;}
    if (minutes < 10) {minutes = "0"+minutes;}
    if (seconds < 10) {seconds = "0"+seconds;}
    return hours+':'+minutes+':'+seconds;
}

function loadSettings() {
    $.ajax({
        type: 'GET',
        url: '/api/settings',
        dataType: 'json',
        success: function (data) {
            $('form#wifi input[name=wifi-ssid]').val(data.wifiSSID);
            $('form#wifi input[name=wifi-password]').val(data.wifiPassword);

            $('form#mqtt input[name=mqttHost]').val(data.mqttHost);
            $('form#mqtt input[name=mqttPort]').val(data.mqttPort);
            $('form#mqtt input[name=mqttLogin]').val(data.mqttLogin);
            $('form#mqtt input[name=mqttPassword]').val(data.mqttPassword);
            $('form#mqtt input[name=mqttCommandTopic]').val(data.mqttCommandTopic);
            $('form#mqtt input[name=mqttStateTopic]').val(data.mqttStateTopic);
            $('form#mqtt input[name=mqttHADiscoveryPrefix]').val(data.mqttHADiscoveryPrefix);

            if (data.mqttIsHADiscovery) {
                $('form#mqtt input[name=mqttIsHADiscovery]').prop('checked', true);
            }
        },
        error: function (xhr, str) {
            alert('Errors while loading settings');
        }
    });
}

function updateWiFiStatus() {
    if (updateWiFiStatusInProgress) {
        return;
    }

    updateWiFiStatusInProgress = true;

    $.ajax({
        type: 'GET',
        url: '/api/status',
        dataType: 'json',
        success: function (data) {
            $('#wifi-status').html(data.wifiStatus);
            $('#uptime').html(toHHMMSS(data.uptime));
            $('#freeHeap').html(data.freeHeap + " bytes");
            $('#lastResetReason').html(data.lastResetReason);
        },
        complete: function() {
            updateWiFiStatusInProgress = false;
        }
    });
}

function updateBoilerStatus() {
    if (updateBoilerStatusInProgress) {
        return;
    }

    updateBoilerStatusInProgress = true;

    $.ajax({
        type: 'GET',
        url: '/api/boiler/stats',
        success: function (data) {
            $('#boiler-status').html(data);
        },
        complete: function() {
            updateBoilerStatusInProgress = false;
        }
    });
}

$(function() {
    loadSettings();
    updateWiFiStatus();
    updateBoilerStatus();

    setInterval(updateWiFiStatus, 10000);
    setInterval(updateBoilerStatus, 10000);

    $('a.nav-link').click(function(e) {
        e.preventDefault();
        var href = $(this).attr('href');
        if (href == "#boiler") {
            $('#boiler').show();
            $('#settings').hide();
        } else {
            $('#boiler').hide();
            $('#settings').show();
        }
    })

    $('form#wifi').submit(function(event) {
        event.preventDefault();

        $.ajax({
            type: 'POST',
            url: '/api/settings/wifi',
            dataType: 'json',
            data: {
                wifiSSID: $(this).find('input[name=wifi-ssid]').val(),
                wifiPassword: $(this).find('input[name=wifi-password]').val()
            },
            success: function (data) {
                alert('Settings successful changed. Reboot...');

                $.ajax({
                    type: 'POST',
                    url: '/api/reboot',
                    dataType: 'json'
                });
            },
            error: function (xhr, str) {
                var data = JSON.parse(xhr.responseText);
                alert('Errors while save settings: ' + data.message);
            }
        });

        return false;
    });


    $('form#mqtt').submit(function(event) {
        event.preventDefault();

        $.ajax({
            type: 'POST',
            url: '/api/settings/mqtt',
            dataType: 'json',
            data: {
                host: $(this).find('input[name=mqttHost]').val(),
                port: $(this).find('input[name=mqttPort]').val(),
                login: $(this).find('input[name=mqttLogin]').val(),
                password: $(this).find('input[name=mqttPassword]').val(),
                haDiscoveryPrefix: $(this).find('input[name=mqttHADiscoveryPrefix]').val(),
                mqttIsHADiscovery: $(this).find('input[name=mqttIsHADiscovery]').is(':checked'),
                commandTopic: $(this).find('input[name=mqttCommandTopic]').val(),
                stateTopic: $(this).find('input[name=mqttStateTopic]').val()
            },
            success: function (data) {
                alert('Settings successful changed. Reboot...');

                $.ajax({
                    type: 'POST',
                    url: '/api/reboot',
                    dataType: 'json'
                });
            },
            error: function (xhr, str) {
                var data = JSON.parse(xhr.responseText);
                alert('Errors while save settings: ' + data.message);
            }
        });

        return false;
    });
});