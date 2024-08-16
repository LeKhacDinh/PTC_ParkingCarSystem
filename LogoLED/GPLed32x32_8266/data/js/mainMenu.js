var BOOT_MODE_NORMAL = 2;
/*if(m_BootMode == BOOT_MODE_NORMAL){
    document.getElementById('btnMyApplication').classList.remove('hide-item');
}else{
    document.getElementById('btnMyApplication').classList.add('hide-item');
}*/
document.getElementById('btnMyApplication').classList.remove('hide-item');

document.getElementById('btnMyWifiRouter').addEventListener('click', function (e) {
    location.replace('/myWifiRouter?sessionId=' + sessionId);
});

document.getElementById('btnMyApplication').addEventListener('click', function (e) {
    location.replace('/myApplication?sessionId=' + sessionId);
});

document.getElementById('btnKetNoiWifiRouter').addEventListener('click', function (e) {
    location.replace('/ketNoiWifiRouter?sessionId=' + sessionId);
});

document.getElementById('logo').addEventListener('click', function (e) {
    location.replace('/mainPage?sessionId=' + sessionId);
});