<?php
/**
 * @file
 * Contains hooks and functions related the field settings.
 */

/**
 * Implements hook_field_widget_form_alter().
 */
function svg_image_field_widget_form_alter(&$element, &$form_state, $context) {
  if ($context['field']['type'] === 'image') {
    $extensions_settings = $context['instance']['settings']['file_extensions'];
    $allowed_extensions = explode(' ', $extensions_settings);

    if (in_array('svg', $allowed_extensions)) {
      foreach ($element as $delta => $field_item) {
        // Field item could be a string for example.
        if (is_array($field_item)
          // Check that nobody delete this validator.
          && !empty($field_item['#upload_validators']['file_validate_extensions'][0])) {
          $extensions_to_validate = explode(' ', $field_item['#upload_validators']['file_validate_extensions'][0]);
          // Add SVG as extension that we allow.
          $extensions_to_validate[] = 'svg';

          $field_item['#upload_validators']['file_validate_extensions'] = array(
            implode(' ', $extensions_to_validate)
          );

          // Rebuild #description with new extensions - it was built
          // in the image_field_widget_form().
          if (!empty($field_item['#description'])) {
            $field_item['#description'] = theme('file_upload_help', array(
              'description' => field_filter_xss($context['instance']['description']),
              'upload_validators' => $field_item["#upload_validators"],
            ));
          }

          $element[$delta] = $field_item;
        }
      }
    }
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function svg_image_field_formatter_info() {
  // Fetch the image formatter info from the core image type.
  $core_image_formatters = image_field_formatter_info();

  $image_field = $core_image_formatters['image'];

  // Add SVG Image related settings.
  $image_field['label'] = t('Image (SVG Support)');
  $image_field['settings']['svg_settings'] = array(
    'width' => '',
    'height' => '',
  );

  return array('svg_image' => $image_field);
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function svg_image_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $element = image_field_formatter_settings_form($field, $instance, $view_mode, $form, $form_state);

  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element['svg_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('SVG Images dimensions (attributes)'),
    '#tree' => TRUE,
  );

  $element['svg_settings']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#size' => 10,
    '#field_suffix' => 'px',
    '#default_value' => $settings['svg_settings']['width'],
  );

  $element['svg_settings']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#size' => 10,
    '#field_suffix' => 'px',
    '#default_value' => $settings['svg_settings']['height'],
  );

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function svg_image_field_formatter_settings_summary($field, $instance, $view_mode) {
  $formatter_settings = $instance['display'][$view_mode]['settings'];
  $original_summary = module_invoke('image', 'field_formatter_settings_summary', $field, $instance, $view_mode);
  $summary = explode('<br />', $original_summary);
  if (!empty($formatter_settings['svg_settings']['width'])
    || !empty($formatter_settings['svg_settings']['width'])) {
    $summary[] = t(
      'SVG images attributes: <br /> Width: @widthpx,<br /> Height: @heightpx',
      array(
        '@width' => (int) $formatter_settings['svg_settings']['width'],
        '@height' => (int) $formatter_settings['svg_settings']['height'],
      )
    );
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function svg_image_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  // Check if the formatter involves a link.
  if ($display['settings']['image_link'] == 'content') {
    $uri = entity_uri($entity_type, $entity);
  }
  elseif ($display['settings']['image_link'] == 'file') {
    $link_file = TRUE;
  }

  foreach ($items as $delta => $item) {
    if (isset($link_file)) {
      $uri = array(
        'path' => file_create_url($item['uri']),
        'options' => array(),
      );
    }
    $attributes = array();
    $is_svg = svg_image_is_svg($item['uri']);

    // Add custom attributes only for SVG images.
    if ($is_svg) {
      $svg_settings = $display['settings']['svg_settings'];

      foreach ($svg_settings as $attribute => $attribute_value) {
        if (!empty($attribute)) {
          $item[$attribute] = (int) $attribute_value;
        }
      }
    }

    $element[$delta] = array(
      '#theme' => 'image_formatter',
      '#item' => $item,
      '#image_style' => $is_svg === FALSE ? $display['settings']['image_style'] : '',
      '#path' => isset($uri) ? $uri : '',
    );
  }

  return $element;
}
