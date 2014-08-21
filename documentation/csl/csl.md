% CSL (complete scripting language)

* Simple parser
* Not yet needed, but should be relatively easy to implement

    bind key a null {set display.fov 200};

will translate into

    struct csl_command_b {
      int action=CSL_COMMAND_BIND;
      struct csl_command_bind_data {
        int key=43; // whatever ascii for 'a' is
        struct csl_command_list_b {
          struct csl_command_b command={
            int action=CSL_COMMAND_SET;
            struct csl_command_set_data {
              char *variable="display.fov";
              union data {
                double dvalue=200;
              };
            };
          };
        };
      };
    } bind_a_fov;

which you could call with

    csl_command_call(bind_a_fov);
.

Other examples:

    bind key a null {if true {say all "howdy-doo!"} {say all "dis shud nevr hapn"}}
