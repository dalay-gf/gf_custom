(function ($) {
    Drupal.behaviors.gfCustomInvite = {
        attach: function (context, settings) {
            // в основном меню меняем ссылку каталога на ссылку страницы новинок.
            $('.nav-main a[href="/ru/shop"]', context).attr("href", '/ru/novelty/2764');
        }
    };
})(jQuery);
