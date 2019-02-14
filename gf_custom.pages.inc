<?php


/**
 * Отправляем программно форму добавления товара в карзину.
 *
 */
function gf_custom_addtocart($node, $ajax) {
  $form_state = [
    'values' => [
      'nid' => $node->nid,
      'qty' => 1,
    ],
  ];

  drupal_form_submit("uc_product_add_to_cart_form", $form_state, $node);

  if ($ajax == 'ajax') {
    // drupal_add_library('system', 'drupal.ajax');
    ajax_deliver(gf_ajax_ubercart_add_to_cart());
    drupal_exit();
  }
  return '';
}
