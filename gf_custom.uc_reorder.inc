<?php

/**
 * Adds all items in a previously created order to the current cart.
 *
 * @param $uid
 *   User ID.
 * @param $order
 *   Order number.
 */
function _gf_custom_uc_reorder($account, $order_id) {
  global $user;

  $uid = $account->uid;

  // Проверяем, кто дублирует заказ.
  $order_uid = db_query('SELECT uid FROM {uc_orders} WHERE order_id = :id', array(':id' => $order_id))->fetchField();
  if (!user_access('view all orders') && $order_uid != $uid) {
    drupal_goto('user/' . $uid . '/orders');
  }

  // Получаем все товары заказа.
  $result = db_query('SELECT nid, qty, data FROM {uc_order_products} WHERE order_id = :id', array(':id' => $order_id));

  foreach ($result as $product) {
    $product->data = unserialize($product->data);

    // Обрабатываем возможные атрибуты товаров.
    $str_attributes = array();
    if (isset($product->data['attributes'])) {
      // Вытаскиваем подмассив атрибутов.
      $str_attributes = $product->data['attributes'];

      // Очищаем подмассив.
      $product->data['attributes'] = array();
    }

    // Проходим по строковым атрибутам для поиска в них id-шников - 
    // будут нужны в дальнейшем при передаче в uc_cart_add_item ().
    foreach ($str_attributes as $attribute => $option_array) {
      // aid is a unique primary key.
      $attribute_id = db_query('SELECT aid FROM {uc_attributes} WHERE label = :label', array(':label' => $attribute))->fetchField();
      foreach ($option_array as $option) {
        // oid is a unique primary key.
        $option_id = db_query('SELECT oid FROM {uc_attribute_options} WHERE aid = :aid AND name = :name', array(':aid' => $attribute_id, ':name' => $option))->fetchField();
        if ($option_id) {
          // Select, Radio, or Checkbox.
          $product->data['attributes'][$attribute_id][] = $option_id;
        }
        else {
          // Textfield.
          $product->data['attributes'][$attribute_id] = check_plain($option);
        }
      }
    }

    uc_cart_add_item($product->nid, $product->qty, $product->data, $uid, TRUE, FALSE);
  }

  // Перенаправляем запустившего процесс дублирования заказа в зависимости
  // от его к нему(заказу) принадлежности.
  if ($user->uid == $uid) {
    $redirect_path = 'cart';
  }
  else {
    drupal_set_message(t('Products from the previous order @order_id added to the user @username.', 
      ['@order_id'=>$order_id, '@username'=>$account->name]));
    $redirect_path = 'user/' . $uid . '/orders';
  }

  drupal_goto($redirect_path);
}
