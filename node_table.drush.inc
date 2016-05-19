<?php

/**
 * Coding Practices for this (demo) Module
 * * NOTE: the coding practices explained below are IMHO, they may or may not be
 *       agreed upon by the Drupal community and/or the Drush community at large
 *       further, they may or may not adhere to Drupal and/or Drush coding standards
 *       but, as we all know, that only starts to matter when you choose to publish
 *       your work, which is beyond the scope of what I am advocating. If you reach that
 *       juncture -- Congratulations! -- and that is the point at which these issues will
 *       start to matter.
 *
 */
/**
 * END Coding Practices for this (demo) Module
 */

/**
 * Drush Command Essential Functions
 * * the function below is the basis of implementing
 *      one or more Drush Commands
 * * the additional text that is required to support the 'drush help {command}' can
 *       get so long and involved it tends to obfuscate the critical 'syntactical' purpose
 *       the $items array;
 *       for this reason, I include only the most trivial descriptive
 *       string below, and implement a specific 'Utility Function',
 *       _render_help_documentation(), to overload and append the content to flesh-out
 *       a robust call to 'drush help {command}'
 */

/**
 * Implements hook_drush_command().
 */
function node_table_drush_command() {
/**
 *
 */
  $items['node-tablelist'] = array(
    'aliases' => array(
      'ntl',
      'notable',
      ),
    'options' => array(
      'context' => 'is option',
      'orderby' => 'is option',
      'limit' => 'is option',
      'list' => 'is option',
      'content_types' => 'is option',
      'dev' => 'is option',
    ),
  );
  $items['fahrenheit-celcius'] = array(
    'aliases' => array(
      'fc',
      ),
    'arguments' => array(
      'degrees_fahrenheit' => 'is argument',
    ),
    'options' => array(
      'context' => 'is option',
    ),
  );
  $items['developer-vardump'] = array(
    'aliases' => array(
      'devv',
      ),
  );

_render_help_documentation($items);

return $items;
}
/**
 * END Drush Command Essential Functions
 */

/**
 * Drush Naming Convention Functions
 * * these function will auto-magically be called based on the drush
 *       naming conventions
 * * they are the starting point for writing the code that implements your command
 * * these should manage only the 'drush specific' aspects of the command, including
 *       but not limited to:
 * * - implementing any drush UI, such as drush_choice()
 * * - validating argumants, aliases, and arguments
 */

/**
 * Callback for the drush-demo-command command
 */
function drush_node_table_node_tablelist($which_list = FALSE) {
  $print = drush_get_option('context', 0);
  $print = $print == 1 ? TRUE : FALSE;
  $context = _render_drush_context($print);
  $options = $context['custom']['options'];
  unset($options['context']); //already evalauted
  $dev = $options['dev'] == 1 ? TRUE : FALSE; //affects below
  $orderby = $options['orderby'] == 'not_provided' ? 'RANDOM' : strtoupper($options['orderby']);
  $orderby = $orderby == 'NODE_ID' ? 'NID' : $orderby; //Alias
  $orderby = $orderby == 'NODEID' ? 'NID' : $orderby; //Alias
  $limit_default = $dev ? 1 : 10;
  $limit = $options['limit'] == 'not_provided' ? $limit_default : $options['limit'];
  $list = $options['list'] == 'not_provided' ? '' : $options['list'];
  /*<Validation Here>*/
  $supported_orderby_array = array('RANDOM','NID','TITLE',);
  /*</Validation Here>*/
  $option_array['limit'] = $limit;
  $option_array['list'] = $list;
  $option_array['content_types'] = $content_types;
  $option_array['orderby'] = $orderby;
  $option_array['dev'] = $dev;
  _do_node_tablelist($option_array);
  return;
//drush_mymodule_custom_save_node
  // KEEP FOR CONTENT_TYPES LATER
    $arguments = _which_list_arguments(); //use custom function to encapsulate
    if (!$which_list) {
        $which_list = drush_choice($arguments, dt('Which Example would you like to interact with using \'PHP-Coder\'?'));
    }

  // Check for correct argument
  $correct_args = array_keys($arguments);
  if (!in_array($which_list, $correct_args)) {
    if ($which_list == '0') {
      drush_user_abort('Buh-Bye! Node Table List!');
      return;
    }
    $string = _which_list_arguments('string');
    return drush_set_error(dt('"@type" is not a valid example. ',
      array('@type' => $example)) . $string);
  }

  drush_print($which_list);
  return;
  // _do_node_tablelist($which_list);
} //END function drush_node_tablelist($which_list = FALSE)


/**
 * Callback for the drush-demo-command command
 */
function drush_node_table_fahrenheit_celcius(){
  $print = drush_get_option('context', 0);
  $print = $print == 1 ? TRUE : FALSE;
  $context = _render_drush_context($print);

  $arg_count = count($context['drush']['arguments']) + 0;
  $degrees_fahrenheit = $context['custom']['arguments']['degrees_fahrenheit'];
  if ($arg_count != 1) {
    return drush_set_error(dt('"@arg_count" is not the expected number of arguments [1 expected].',
      array('@arg_count' => $arg_count)));
  }
  $degrees_fahrenheit = str_replace('/', '', $degrees_fahrenheit);
  if (!is_numeric($degrees_fahrenheit)) {
    return drush_set_error(dt('Argument must be numeric ["@degrees_fahrenheit" is not numeric].',
      array('@degrees_fahrenheit' => $degrees_fahrenheit)));
  }
  $degrees_fahrenheit += 0;
  if ($degrees_fahrenheit < -459.67) {
    return drush_set_error(dt('Argument is out of range ["@degrees_fahrenheit" degrees Fahrenheit is is below absolute zero].',
      array('@degrees_fahrenheit' => $degrees_fahrenheit)));
  }
_do_convert_fahrenheit_to_celcius($degrees_fahrenheit);
}

/**
 * Callback for the drush-demo-command command
 */
function drush_node_table_developer_vardump() {
  _do_developer_vardump();
}
/**
 * END Drush Naming Convention Functions
 */

/**
 * Drush Utility Functions
 * * functions here may facilitate one or more aspects of compo
 *
 */
function _which_list_arguments($which = 'array'){
  $which_arguments = array();
  $which_arguments['list'] = "Help Choice for Node Table List 'List' argument.";
  $which_arguments['groupby'] = "Help Choice for Node Table List 'GroupBy' argument.";
  $which_arguments['cube'] = "Help Choice for Node Table List 'Cube' argument.";

  switch ($which) {
    case 'array':
      return $which_arguments;
      break;
    case 'string':
      $correct_args = array_keys($which_arguments);
      $correct_args_count = count($correct_args);
      $first_args = $correct_args;
      $last_arg = '"' .  array_pop($first_args) . '"';
      $first_args_list = implode(",", $first_args);
      $first_args_list = '"' . str_replace(",", '", "', $first_args_list) . '"';
      $zero = 'Please ask for assistance, there are currently no php_coder examples in use';
      $non_zero = 'Please select';
      $grammar = $correct_args_count > 2?' among ':': ';
      $grammar = $correct_args_count == 2?' between ':$grammar;
      $list = '';
      $list = $correct_args_count == 1?$last_arg:$list;
      $list = $correct_args_count > 1?$first_args_list. ' and ' . $last_arg:$list;
      if ($correct_args_count == 0) {
        $string = $zero;
      }else{
        $string = $non_zero . $grammar . $list . '. (Or supply no example and select from prompted values.)';
      }
      return $string;
      break;

    default:
      return $which_arguments;
      #\_ OOAAOC notwithstanding default $which, this is its raison detre
      break;
  }
  return; //OOAAOC
}

function _render_help_documentation(&$items) {
/**
  * $items Append & Overload below
  * description ONLY here
  * examples ONLY here
  * aliases isn't associative so NEVER here
  * others fleshed out for HELP command
  */


  _render_help_documentation_default($items);
}
function _render_help_documentation_default(&$items) {
  foreach ($items as $command => $command_attributes) {
    if (empty($command_attributes['description'])) {
      $items[$command]['description'] = "Description for the drush command '{$command}'.";
    }
    $descr_example['Description as First Example'] = $items[$command]['description'];
    if (is_array($items[$command]['examples'])) {
      $items[$command]['examples'] = $descr_example + $items[$command]['examples'];
    }else{
      $items[$command]['examples'] = $descr_example;
    }
    if (is_array($command_attributes['arguments'])) {
      foreach ($command_attributes['arguments'] as $argument => $argument_descr) {
        if ($argument_descr == 'is argument') {
          $items[$command]['arguments'][$argument] = "Longer description of Argument '{$argument}'";
        }
      }
    }
    if (is_array($command_attributes['options'])) {
      foreach ($command_attributes['options'] as $option => $option_descr) {
        if ($option_descr == 'is option') {
          $items[$command]['options'][$option] = "Longer description of Option '{$option}'";
        }
      }
    }
  }
}
function _render_drush_context($print = FALSE) {
  $print = $print !== TRUE ? FALSE : TRUE;

  $context = drush_get_context();
  $command = $context['command'];

  $argument_values = $command['arguments'];
  $argument_keys = array_keys($command['argument-description']);
  $count_argument_values = count($argument_values);
  $count_argument_keys = count($argument_keys);
  if ($count_argument_keys > $count_argument_values) {
    foreach ($argument_keys as $index => $key) {
      if (empty($argument_values[$index])) {
        $argument_values[$index] = "missing_argument";
      }
    }
  }
  if ($count_argument_values > $count_argument_keys) {
    foreach ($argument_values as $index => $value) {
      if (empty($argument_keys[$index])) {
        $argument_keys[$index] = "extra_arg_{$index}";
      }
    }
  }
  $option_keys = $command['options'];
  $option_values = $context['cli'];
  foreach ($option_keys as $key => $descr) {
    $option_array[$key] = empty($option_values[$key]) ? 'not_provided' : $option_values[$key];
  }


  $developer_friedly_array = array();
  $dfa = array();
  $dfa_drush = array();
  $dfa_drush['command'] = $command['command'];
  $dfa_drush['argument-description'] = $command['argument-description'];
  $dfa_drush['arguments'] = $command['arguments'];
  $dfa_drush['options'] = $command['options'];
  $dfa_drush['cli'] = $context['cli'];
  $dfa_drush['aliases'] = $command['aliases'];
  $dfa_custom = array();
  $hook = 'drush_' . $command['commandfile'] . '_';
  $hook .= str_replace('-', '_', $command['command-hook']) . '()';
  $dfa_custom['command_hook_function'] = $hook;
  $dfa_custom['arguments'] = array_combine($argument_keys, $argument_values);
  $dfa_custom['options'] = $option_array;
  $developer_friedly_array['drush'] = $dfa_drush;
  $developer_friedly_array['custom'] = $dfa_custom;
  if ($print) {
    $header = "Output from '" . __FUNCTION__ . "()':\r\n======================================\r\n";
    $output = print_r($developer_friedly_array, TRUE);
    drush_print($header . $output);
  }
  return $developer_friedly_array;
}
/**
 * END Drush Utility Functions
 */


/**
 * Drush Specific Functions
 * * including functions that are called by the 'Drush Naming Convention Functions'
 *   \_ aka the '_do()' functions
 * * the 'Drush Naming Convention Fuctions' :
 * * - are the functions that are called automatically by way of the the drush command name
 *
 */
/**
 * Example function.
 */

function _do_node_tablelist($options_array = array()) {
    drush_print(print_r($options_array, TRUE));
    $node_array = _gather_node_object_array($options_array);

    if ($dev === TRUE) {
      $output = print_r($node_array, TRUE);
      drush_print($output);
    }else{
      $i = 0;
      $column_array = array('nid', 'type', 'title');
      foreach ($node_array as $nid => $node_object) {
        if ($i == 0) {
          $rows[$i]['rownum'] = '#';
          foreach ($column_array as $index => $column) {
            $rows[$i][$column] = ucwords($column);
          }
        }
        $i++;
        $rows[$i]['rownum'] = $i;
        foreach ($column_array as $index => $column) {
          $rows[$i][$column] = $node_object->$column;
        }
      }
    $header = 'Node Listing';
    drush_print_table($rows, $header);
      // $output = print_r($node_array, TRUE);
      // drush_print($output);
    }
}

function _do_convert_fahrenheit_to_celcius($degrees_fahrenheit) {
  $degrees_celcius = $degrees_fahrenheit;
  $degrees_celcius -= 32;
  $degrees_celcius *= 5;
  $degrees_celcius /= 9;
  $degrees_celcius = round($degrees_celcius,2);

  $degrees_to_celcius_string = "{$degrees_fahrenheit} degrees Fahrenheit\r\nis equal to\r\n{$degrees_celcius} degrees Celcius";
  drush_print($degrees_to_celcius_string);
}

function _do_developer_vardump() {
  $header = "Output from '" . __FUNCTION__ . "()':\r\n==========\r\n";
  $output = 'Hello Drupal Camp Twin-Cities 2016!';//'Hello World!';

  // $thing_array = array();
  // $i = -1;
  // $i++;
  // $thing[$i]['thing'] = 'Corvette';
  // $thing[$i]['type'] = 'Little';
  // $thing[$i]['color'] = 'Red';
  // $i++;
  // $thing[$i]['thing'] = 'Meanies';
  // $thing[$i]['type'] = 'Screaming';
  // $thing[$i]['color'] = 'Blue';
  // $i++;
  // $thing[$i]['thing'] = 'Fox';
  // $thing[$i]['type'] = 'Quick';
  // $thing[$i]['color'] = 'Brown';
  // $output = print_r($thing, TRUE);

  drush_print($header . $output);
}

/**
 * END Drush Specific Functions
 */
