(function ($) {
    Drupal.behaviors.load_accounting = {
        attach: function (context, settings) {

            $("#btnChangeNickname").click(function(){
               $("#miniform-nickname").modal();
            });

            $("#btnChangeCompensation").click(function(){
                $("#miniform-compensation").modal();
            });

            var avoidEnter = function (e)
            {
                var keyCode = e.keyCode ? e.keyCode : e.which ? e.which : e.charCode;
                return keyCode != 13;
            };

            var strpos = function (haystack,needle,offset){
                var i=(haystack+'').indexOf(needle,(offset||0));
                return i===-1?false:i;
            };

            var validateFloat = function(e){
                var value = e.target.value;
                var keyCode = e.keyCode ? e.keyCode : e.which ? e.which : e.charCode;

                if ((keyCode < 48 || keyCode > 60) && keyCode != 8 && keyCode != 190 && keyCode != 39 && keyCode != 37 && keyCode != 46 && keyCode != 9)
                    return false;

                if (strpos(value, '.') != false && keyCode == 190)
                    return false;

                return true;
            };

            $("#driver-compensation-type").keydown(avoidEnter);

            $("#driver-compensation-value").keydown(function(e){
                if (!avoidEnter(e)) return false;
                return validateFloat(e);
            });

            $("#truck-nickname").keydown(avoidEnter);

            $(".btn-upload-bol").click(function(){
                var id = "#miniform-upload-"+($(this).attr('id').replace('btn-upload-',''));
                //alert(id);
                $(id).modal();
            });
        }
    };
})(jQuery);

