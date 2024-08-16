document.getElementById('lblDeviceId').innerText = m_DeviceId;
document.getElementById('ssid').value = m_SSID;
document.getElementById('password').value = m_Password;
document.getElementById('login_id').value = m_LoginId;
document.getElementById('login_password').value = m_LoginPassword;
document.getElementById('min_distance').value = m_MinDistance;
document.getElementById('max_distance').value = m_MaxDistance;

document.getElementById('name').value = m_DeviceName;
if(m_IpType != IP_TYPE_MANUAL){
    document.getElementById('dhcp').checked = true;
}else{
    document.getElementById('manual-ip').checked = true;
    document.getElementById('manual-ip-session').classList.remove('hide-item');
    document.getElementById('ipAddress').value = m_Ip;
    document.getElementById('gateway').value = m_Gateway;
    document.getElementById('subnet').value = m_SubnetMask;
}

document.getElementById('mnu-wifi').addEventListener('click', function (e) {
    e.preventDefault();
    if(this.classList.contains('active')) return;
    document.querySelector('.active').classList.remove('active');
    this.classList.add('active');
    document.getElementById('wifi-session').classList.remove('hide-item');
    document.getElementById('device-session').classList.add('hide-item');
    document.getElementById('security-session').classList.add('hide-item');
});

document.getElementById('mnu-device').addEventListener('click', function (e) {
    e.preventDefault();
    if(this.classList.contains('active')) return;
    document.querySelector('.active').classList.remove('active');
    this.classList.add('active');
    document.getElementById('wifi-session').classList.add('hide-item');
    document.getElementById('device-session').classList.remove('hide-item');
    document.getElementById('security-session').classList.add('hide-item');
});

document.getElementById('mnu-security').addEventListener('click', function (e) {
    e.preventDefault();
    if(this.classList.contains('active')) return;
    document.querySelector('.active').classList.remove('active');
    this.classList.add('active');
    document.getElementById('wifi-session').classList.add('hide-item');
    document.getElementById('device-session').classList.add('hide-item');
    document.getElementById('security-session').classList.remove('hide-item');
});

document.getElementById('btnRestart').addEventListener('click', function (e) {
    location.replace('/restart');
});

document.getElementById('btnSaveSetting').addEventListener('click', function (e) {
    var manualIp = document.getElementById('manual-ip').checked;
    if(manualIp){
        var ip = document.getElementById('ipAddress').value;
        if(!isValidIPv4(ip)){
            alert('Nhập sai ip, vui lòng kiểm tra lại.');
            return;
        }
        var gateway = document.getElementById('gateway').value;
        if(!isValidIPv4(gateway)){
            alert('Nhập sai gateway, vui lòng kiểm tra lại.');
            return;
        }
        var subnetmask = document.getElementById('subnet').value;
        if(!isValidIPv4(subnetmask)){
            alert('Nhập sai subnet mask, vui lòng kiểm tra lại.');
            return;
        }
    }
    document.getElementById('formSaveSetting').submit();
});

document.getElementById('btnWifiRefresh').addEventListener('click', function (e) {
    var xhr = new XMLHttpRequest();
    var url = '/wifiRefresh';
    xhr.open('POST', url, true);
    xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    xhr.onreadystatechange = function() {
        if (xhr.readyState === 4 && xhr.status === 200) {
            select.innerHTML = xhr.responseText;
            select = document.getElementById('ssid');
        }
    };
    xhr.send();
});

document.getElementById('logo').addEventListener('click', function (e) {
    location.replace('/mainPage?sessionId=' + sessionId);
});

document.getElementById('dhcp').addEventListener('click', function (e) {
    document.getElementById('manual-ip-session').classList.add('hide-item');
});

document.getElementById('manual-ip').addEventListener('click', function (e) {
    document.getElementById('manual-ip-session').classList.remove('hide-item');
});

function isValidIPv4(ip) {
    var ipv4Regex = /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
    return ipv4Regex.test(ip);
}

function generateGatewayAndSubnetMask() {
    var ipAddress = document.getElementById('ipAddress').value;
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
        document.getElementById('gateway').value = gateway;
        document.getElementById('subnet').value = subnetMask;
    }
}
