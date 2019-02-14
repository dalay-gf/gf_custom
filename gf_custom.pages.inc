<?php


/**
 * Отправляем программно форму добавления товара в карзину.
 *
 */
function gf_custom_addtocart($node) {
  $form_state = [
    'values' => [
      'nid' => $node->nid,
      'qty' => 1,
    ],
  ];

  drupal_form_submit("uc_product_add_to_cart_form", $form_state, $node);
  return '';
}
