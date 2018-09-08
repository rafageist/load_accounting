var avoidEnter = function (e) {
    var keyCode = e.keyCode ? e.keyCode : e.which ? e.which : e.charCode;
    return keyCode != 13;
};

var strpos = function (haystack, needle, offset) {
    var i = (haystack + '').indexOf(needle, (offset || 0));
    return i === -1 ? false : i;
};

var validateFloat = function (e) {
    var value = e.target.value;
    var keyCode = e.keyCode ? e.keyCode : e.which ? e.which : e.charCode;

    if ((keyCode < 48 || keyCode > 60) && keyCode != 8 && keyCode != 190 && keyCode != 39 && keyCode != 37 && keyCode != 46 && keyCode != 9)
        return false;

    if (strpos(value, '.') != false && keyCode == 190)
        return false;

    return true;
};

var validateFloatAndEnter = function (e) {
    if (!avoidEnter(e)) return false;
    return validateFloat(e);
};

var number_format = function (number, decimals, dec_point, thousands_sep) {
    var n = !isFinite(+number) ? 0 : +number, prec = !isFinite(+decimals) ? 0 : Math.abs(decimals),
        sep = (typeof thousands_sep === 'undefined') ? ',' : thousands_sep,
        dec = (typeof dec_point === 'undefined') ? '.' : dec_point, s = '',
        toFixedFix = function (n, prec) {
            var k = Math.pow(10, prec);
            return '' + Math.round(n * k) / k;
        };

    s = (prec ? toFixedFix(n, prec) : '' + Math.round(n)).split('.');
    if (s[0].length > 3) {
        s[0] = s[0].replace(/\B(?=(?:\d{3})+(?!\d))/g, sep);
    }
    if ((s[1] || '').length < prec) {
        s[1] = s[1] || '';
        s[1] += new Array(prec - s[1].length + 1).join('0');
    }
    return s.join(dec);
};

(function ($) {
    Drupal.behaviors.load_accounting = {
        attach: function (context, settings) {

            $("#btnChangeNickname").click(function () {
                $("#miniform-nickname").modal();
            });

            $("#btnChangeCompensation").click(function () {
                $("#miniform-compensation").modal();
            });

            $("#driver-compensation-type").keydown(avoidEnter);
            $("#driver-compensation-value").keydown(validateFloatAndEnter);
            $("#truck-nickname").keydown(avoidEnter);

            $(".btn-upload-bol").click(function () {
                var id = "#miniform-upload-" + ($(this).attr('id').replace('btn-upload-', ''));
                $(id).modal();
            });

            var modalConfirm = function (title, text, submitValue, submitFlag) {
                $("#confirm-title").html(Drupal.t(title));
                $("#confirm-text").html(Drupal.t(text));
                $("#confirm-dialog-submit").val(submitValue);
                $("#confirm-dialog-flag").val(submitFlag);
                $("#confirm-dialog").modal();
            };

            $(".btn-delete-payment").click(function () {
                var id = $(this).attr('data-payment-id');
                modalConfirm('Confirmation', 'Associated loads will be set as unpaid. You cannot undo this action. Are you sure you want to delete this payment?', id, 'delete-payment');
            });

            $("#btn-delete-bulk-payments").click(function () {
                modalConfirm('Confirmation', 'Associated loads will be set as unpaid. You cannot undo this action. Are you sure you want to delete these payments?', 'bulk', 'delete-bulk-payment');
            });

            $(".btn-edit-payment").click(function () {
                var id = $(this).attr('data-payment-id');
                var miles = $(this).attr('data-payment-miles') * 1;
                var payment = $(this).attr('data-payment-payment') * 1;
                var memo = $(this).attr('data-payment-memo');
                var accessories = $(this).attr('data-payment-accessories') * 1;
                var date = $(this).attr('data-payment-date');

                $("#payment-edit-form").modal();
                $("#payment-id").val(id);
                $("#payment-miles").val(miles);
                $("#payment-payment").val(payment);
                $("#payment-accessories").val(accessories);
                $("#payment-memo").val(memo);
                $("#payment-date").val(date);

                var updatePaymentValues = function () {
                    var payment = $("#payment-payment").val() * 1;
                    var accessories = $("#payment-accessories").val() * 1;
                    var miles = $("#payment-miles").val() * 1;
                    var total = payment + accessories;
                    var rate = total / miles;

                    $("#payment-total").val(number_format(total, 2));
                    $("#payment-rate").val(number_format(rate, 2));
                };

                $("#payment-miles").keydown(validateFloatAndEnter);
                $("#payment-payment").keydown(validateFloatAndEnter);
                $("#payment-accessories").keydown(validateFloatAndEnter);

                $("#payment-reference").html('Payment #' + $(this).attr('data-payment-reference'));

                $("#payment-miles").keyup(updatePaymentValues);
                $("#payment-payment").keyup(updatePaymentValues);
                $("#payment-accessories").keyup(updatePaymentValues);

                updatePaymentValues();

                function makeFocusHandler(e) {
                    if (!$(this).hasClass('date-popup-init')) {
                        var datePopup = e.data;

                        switch (datePopup.func) {
                            case 'datepicker':
                                $(this)
                                    .datepicker(datePopup.settings)
                                    .addClass('date-popup-init');
                                $(this).click(function () {
                                    $(this).focus();
                                });
                                break;
                        }
                    }
                }

                $('#payment-date').focus(function (e) {
                    $(this)
                        .datepicker({
                            autoPopup: "focus",
                            changeMonth: true,
                            changeYear: true,
                            closeAtTop: false,
                            dateFormat: "mm/dd/yy",
                            defaultDate: "0y",
                            firstDate: 0,
                            fromTo: false,
                            speed: "immediate",
                            yearRange: "+0:+1",
                            value: date
                        })
                        .addClass('date-popup-init');

                    $(this).click(function () {
                        $(this).focus();
                    });
                });
            });
        }
    };
})(jQuery);

