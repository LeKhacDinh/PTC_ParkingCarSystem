document.getElementById('my_app_name').value = my_app_name;
document.getElementById('my_app_ip').value = my_app_ip;
document.getElementById('my_app_min_distance').value = my_app_min_distance;
document.getElementById('my_app_max_distance').value = my_app_max_distance;
document.getElementById('my_app_max_minute').value = my_app_max_minute;
//todo

function getSensorInfo(continueRun){
    $.ajax({
		url: '/getSensorInfo?sessionId=' + sessionId,
		dataType: 'json',
		type: 'POST',
		data: JSON.stringify({
            // Include your JSON data here
            sessionId: sessionId
        }),
        contentType: 'application/json',
		cache: false,
		complete: function() {
		},
		success: function(response, status)
		{
            $('#my_app_current_distance').empty().append(response.distance);
            if(response.kq == 1){
                $('#my_app_kq_xac_dinh').empty().append("Có xe (occupied)");
            }else{
                $('#my_app_kq_xac_dinh').empty().append("Không có xe");
            }

            if(response.from && response.from>0){
                var date = secondsSince2000ToDate(response.from);
                $('#my_app_from').empty().append(formatDateTime(date));
            }else{
                $('#my_app_from').empty().append("");
            }

            if(response.to && response.to>0){
                var date = secondsSince2000ToDate(response.to);
                $('#my_app_to').empty().append(formatDateTime(date));
            }else{
                $('#my_app_to').empty().append("");
            }

            $('#my_app_tong_tg').empty().append(response.total_time);
            if(response.den_canh_bao == 1){
                $('#my_app_den_canh_bao').empty().append("ON").addClass('on');
            }else{
                $('#my_app_den_canh_bao').empty().append("OFF").removeClass('on');;
            }
            var history = response.lich_su;
            if(history){
                const arrs = response.lich_su.split(";")
                var historyList = $('#idLichSuGD').empty();
                var id = 0;
                for(var i=0;i<arrs.length; i++){
                    const item = jQuery.trim(arrs[i]);
                    if(item != ""){
                        if(item.startsWith("date:")){
                            id = 1;
                            const txt = item.substring(5);
                            var iappendItem = $("<div><label></label></div>");
                            iappendItem.find("label").append(txt);
                            historyList.append(iappendItem);
                        }else{
                            var iappendItem = $("<div></div>");
                            iappendItem.append("<" + id + ">" + item);
                            historyList.append(iappendItem);
                            id++;
                        }
                    }
                }
            }
            
            if(continueRun==true){
                setTimeout(function(){
                    getSensorInfo();
                }, 60000);
            }
		},
		error: function(request, status, error) {
            if(continueRun==true){
                setTimeout(function(){
                    getSensorInfo();
                }, 60000);
            }
		}
	});
}

getSensorInfo(true);


function formatDateTime(date) {
    const day = String(date.getDate()).padStart(2, '0');
    const month = String(date.getMonth() + 1).padStart(2, '0'); // Months are 0-based
    const year = date.getFullYear();
    const hours = String(date.getHours()).padStart(2, '0');
    const minutes = String(date.getMinutes()).padStart(2, '0');
    const seconds = String(date.getSeconds()).padStart(2, '0');

    return `${day}/${month}/${year} ${hours}:${minutes}:${seconds}`;
}

$('#btnMyApplicationRefresh').off().on('click', function(){
    getSensorInfo(false);
});

function saveSensorInfo(){
    const min_distance = $('#my_app_min_distance').val();
    const max_distance = $('#my_app_max_distance').val();
    const max_minute = $('#my_app_max_minute').val();
    if(min_distance == "" || isNaN(min_distance)){
        alert('Hãy nhập thông tin điều kiện xác định có xe. (Từ)');
        return;
    }
    if(max_distance == "" || isNaN(min_distance)){
        alert('Hãy nhập thông tin điều kiện xác định có xe. (Đến)');
        return;
    }
    if(max_minute == "" || isNaN(max_minute)){
        alert('Hãy nhập số phút tối đa.');
        return;
    }
    $.ajax({
		url: '/saveSensorInfo?sessionId=' + sessionId,
		dataType: 'json',
		type: 'POST',
		data: JSON.stringify({
            my_app_min_distance: min_distance,
            my_app_max_distance: max_distance,
            my_app_max_minute: max_minute
        }),
        contentType: 'application/json',
		cache: false,
		complete: function() {
		},
		success: function(response, status)
		{
            if(!response){
				alert("There was an unknown error. Please try again. If the problem persists, please contact our technical support.");
				return;
			}
            alert("Save successfull");
		},
		error: function(request, status, error) {
            alert(error);
		}
	});
}

$('#btnMyApplicationSave').off().on('click', function(){
    saveSensorInfo();
});

$('#logo').off().on('click', function (e) {
    location.replace('/mainPage?sessionId=' + sessionId);
});

function secondsSince2000ToDate(seconds) {
    // Create a Date object for January 1, 2000
    const baseDate = new Date(Date.UTC(2000, 0, 1));
    
    // Add the given number of seconds to the base date
    const resultDate = new Date(baseDate.getTime() + (seconds * 1000));
    
    return resultDate;
}