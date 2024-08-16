document.getElementById('wifi_router_name').value = wifiRouterName;
document.getElementById('wifi_router_ip').value = wifiRouterIp;
document.getElementById('wifi_router_password').value =wifiRouterPassword;
document.getElementById('wifi_router_gateway').value = wifiRouterGateway;
document.getElementById('wifi_router_subnetmask').value = wifiRouterSubnetmask;

document.getElementById('btnResetToDefault').addEventListener('click', function (e) {
    document.getElementById('wifi_router_name').value = AP_SSID;
    document.getElementById('wifi_router_ip').value = AP_IP_ADDRESS;
    document.getElementById('wifi_router_password').value = AP_PASWORD;
    document.getElementById('wifi_router_gateway').value = AP_GATEWAY;
    document.getElementById('wifi_router_subnetmask').value = AP_SUBNETMASK;
});

document.getElementById('btnMyWifiRouterSave').addEventListener('click', function (e) {
    var name = document.getElementById('wifi_router_name').value;
    if(name == ''){
        alert('Chưa nhập WIFI Router.');
        return;
    }
    var ip = document.getElementById('wifi_router_ip').value;
    if(!isValidIPv4(ip)){
        alert('Nhập sai ip, vui lòng kiểm tra lại.');
        return;
    }
    var gateway = document.getElementById('wifi_router_gateway').value;
    if(!isValidIPv4(gateway)){
        alert('Nhập sai gateway, vui lòng kiểm tra lại.');
        return;
    }
    var subnetmask = document.getElementById('wifi_router_subnetmask').value;
    if(!isValidIPv4(subnetmask)){
        alert('Nhập sai subnet mask, vui lòng kiểm tra lại.');
        return;
    }
    var password = document.getElementById('wifi_router_password').value;
    if(password == ''){
        alert('Chưa nhập WIFI Passwords.');
        return;
    }
    document.getElementById('formMyWifiRouterSave').submit();
});

function isValidIPv4(ip) {
    var ipv4Regex = /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
    return ipv4Regex.test(ip);
}

function generateGatewayAndSubnetMask() {
    var ipAddress = document.getElementById('wifi_router_ip').value;
    if(isValidIPv4(ipAddress)){
        var octets = ipAddress.split('.');
        var gateway = octets.slice(0, 3).join('.') + '.1';
        var subnetMask = '';
        var firstOctet = parseInt(octets[0]);
        if (firstOctet >= 1 && firstOctet <= 126) {
            subnetMask = '255.0.0.0';
        } else if (firstOctet >= 128 && firstOctet <= 191) {
            subnetMask = '255.255.0.0';
        } else if (firstOctet >= 192 && firstOctet <= 223) {
            subnetMask = '255.255.255.0';
        } else {
            subnetMask = '';
        }
        document.getElementById('wifi_router_gateway').value = gateway;
        document.getElementById('wifi_router_subnetmask').value = subnetMask;
    }
}

document.getElementById('btnLogout').addEventListener('click', function (e) {
    location.replace('/logout?sessionId=' + sessionId);
});

document.getElementById('logo').addEventListener('click', function (e) {
    location.replace('/mainPage?sessionId=' + sessionId);
});