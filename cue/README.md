submodule/    # Contains simple example importing a package from within the module


cue import . -p mypackage --list  # Generate *.cue files from existing *.yaml
cue eval $path                    # Evaluate configs
cue mod tidy                      # Clean up the module definition
cue mod init $module_name         # Initialize module
cue mod fix                       # Fix the module files
cue cmd gen                       # Generate files as specified in gen_tool.cue
cue vet $path                     # validate config
