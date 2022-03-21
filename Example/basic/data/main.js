
(function ($) {
    "use strict";


    /*==================================================================
    [ Focus input ]*/
    $('.input100').each(function(){
        $(this).on('blur', function(){
            if($(this).val().trim() != "") {
                $(this).addClass('has-val');
            }
            else {
                $(this).removeClass('has-val');
            }
        })    
    })
    /*==================================================================
    [ Validate ]*/
    var input = $('.validate-input .input100');

    $('.validate-form').on('submit',function(){
        
        if($('input[type="checkbox"]').prop("checked") == true){
            var check = true;
            for(var i=0; i<input.length; i++) {
                if(validate(input[i]) == false){
                    showValidate(input[i]);
                    check=false;
                }
            }
            return check;
        };

        
    });


    $('.validate-form .input100').each(function(){
        $(this).focus(function(){
        hideValidate(this);
        });
    });

    function validate (input) {
        if($(input).attr('type') == 'password' || $(input).attr('name') == 'pass') {
            if($(input).val().length < 8) {
                return false;
            }
        }
    } 



    function showValidate(input) {
        var thisAlert = $(input).parent();
        $(thisAlert).addClass('alert-validate');
    }

    function hideValidate(input) {
        var thisAlert = $(input).parent();
        $(thisAlert).removeClass('alert-validate');
    }

    //Remember full list max
    /*==================================================================
    [ Show pass ]*/
    var showPass = 0;
    $('.btn-show-pass').on('click', function(){
        if(showPass == 0) {
            $(this).next('input').attr('type','text');
            $(this).find('i').removeClass('zmdi-eye');
            $(this).find('i').addClass('zmdi-eye-off');
            showPass = 1;
        }
        else {
            $(this).next('input').attr('type','password');
            $(this).find('i').addClass('zmdi-eye');
            $(this).find('i').removeClass('zmdi-eye-off');
            showPass = 0;
        }
    });
    

    $(document).ready(function(){
        //Create tab menu
        
        
        $('.tabs-menu a').click(function(event) {
            event.preventDefault();
          // Toggle active class on tab buttons
            $(this).parent().addClass("Current");
            $(this).parent().siblings().removeClass("Current");
          // display only active tab content
            var activeTab = $(this).attr("href");
            $('.TabContent').not(activeTab).css("display","none");
            $(activeTab).fadeIn();
        });


        //Create table content
        $.getJSON("secrets.json", function(data){
            var wifi_data= '';
            var order=0;
            $.each(data, function(key, value){
                order= parseInt(key)+1;
                wifi_data +='<tr>';
                wifi_data += '<td>'+ order +'</td>';
                wifi_data += '<td>'+ value.ssid +'</td>';
                wifi_data += '<td>'+ value.pass + '</td>';
                wifi_data += '<td>' + '<i class="zmdi zmdi-delete">'+ '</i>' +'</td>';
                wifi_data += '</tr>';
            });
            $('#wifi-table').append(wifi_data);
        });
        //Wifi list function
        // $('select[name="SSID"]').click(function(){

        // });
        $('input[type="checkbox"]').click(function(){
            if($(this).prop("checked") == true){
                $("#wifi-credential").removeClass('hide');
            }
            else if($(this).prop("checked") == false){
                $("#wifi-credential").addClass('hide');
                $(this).closest('form').find("input[type=password], textarea").val("");
                $(this).closest('form').find("select[name='SSID'], textarea").val("");


            }
        });
             //Delete row function
            $.getJSON("wifiScan.json", function(obj){
                if (obj.scan_result.length){
                    var htmlSrc = '';
                    for (var i=0; i< 5; i++) {
                        htmlSrc += '<option '+'value='+'"'+obj.scan_result[i].SSID +'">'+obj.scan_result[i].SSID + obj.scan_result[i].RSSI + '%</option>'; 
                    };
                    $('select[name="SSID"]').append(htmlSrc);
                };
            });
        
        
        






    });

})(jQuery);