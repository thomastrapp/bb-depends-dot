# Author: Thomas Trapp - https://thomastrapp.com/
# License: MIT

# This ragel machine extracts string pairs from dot files as generated by
# running `bitbake -g`.
# No attempt is made to honor the rules of the dot language.
# Given the following input line, this machine will extract "recipe1"
# and "recipe2":
# "(recipe1).task" -> "(recipe2).task"
# Lines not matching this format are silently discarded.
%%{
machine dot;

recipe = (
  ( [a-zA-Z0-9\-][a-zA-Z0-9\-]* )
  >{ recipe_name_start(); }
  %{ recipe_name_stop(); }
);
task_suffix = '.' [^"]*;

recipe_and_task = (
  '"' recipe task_suffix? '"'
);

consume_line := [^\n]* '\n' @{ fgoto main; };

main := (
  (
    space*
    recipe_and_task
    %{ push_recipe(0); }
    ' '*
    '->'
    ' '*
    recipe_and_task
    %{ push_recipe(1); }
  )
  %{ push_dependency(); }
  @err{ fhold; fgoto consume_line; }
)**;

}%%
