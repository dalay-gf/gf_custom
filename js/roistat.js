(function ($) {

    Drupal.behaviors.initColorboxInline = {
        attach: function (context, settings) {
            var form = $('form[action$="invite"], form#user-register-form', context);
            var page = window.location.hostname + window.location.pathname;

            form.submit(function(e) {
                var data = {};
                $this = $(this);
                // e.preventDefault();
                $this.find('input, textearea, select').each(function() {
                    // добавим новое свойство к объекту $data
                    // имя свойства – значение атрибута name элемента
                    // значение свойства – значение свойство value элемента
                    data[this.name] = $(this).val();
                });

                if (form[0].id.startsWith('webform-uuid')) { // форма со страницы invite
                    var phone = data['submitted[phone_number]'];
                    var email = data['submitted[e_mail]'];
                    var title = Drupal.t('New request from @page', {'@page': page} );
                } 
                else { // форма со страницы регистрации
                    var phone = data['field_telefon[und][0][value]'];
                    var email = data['mail'];
                    var title = Drupal.t('New registration on @page', {'@page': page});

                    var username = data['field_firstname[und][0][value]'];
                    var registration = "registration";
                }

                var res = { // общие, для всех форм, поля
                    leadName: title,
                    email: email,
                    phone: phone,
                    roistat: getCookiem('roistat_visit'),
                    fields: {
                        TITLE: title,
                        ASSIGNED_BY_ID: 129
                    }
                };

                if (registration) { 
                    // доп. поля для формы регистрации
                    res['fields']['UF_CRM_1543328099'] = registration; 
                    res['name'] = username;
                }

                roistatGoal.reach(res);


            });

        }
    };

})(jQuery);


function getCookiem(name) {
    var matches = document.cookie.match(new RegExp(
        "(?:^|; )" + name.replace(/([\.$?*|{}\(\)\[\]\\\/\+^])/g, '\\$1') + "=([^;]*)"
    ));
    return matches ? decodeURIComponent(matches[1]) : '';
}
