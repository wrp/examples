/*
Demonstration of cobra for flag parsing.

Features shown:
  - Short and long flags (-n / --arg-num)
  - Required flags
  - Persistent flags (inherited by subcommands)
  - Mutually exclusive flags
  - Subcommands
  - Proper error handling (no usage dump on errors)
  - Usage/help written to stdout
*/

package main

import (
	"fmt"
	"os"
	"strings"

	"github.com/spf13/cobra"
)

var verbose bool

func newRootCmd() *cobra.Command {
	root := &cobra.Command{
		Use:           "flag",
		Short:         "Demonstrate cobra features",
		SilenceUsage:  true,
		SilenceErrors: true,
		RunE: func(cmd *cobra.Command, args []string) error {
			return fmt.Errorf("a subcommand is required; see '%s --help'", cmd.CommandPath())
		},
	}
	root.SetOut(os.Stdout)
	root.SetErr(os.Stderr)
	tmpl := root.UsageTemplate()
	tmpl = strings.Replace(tmpl,
		"{{if .Runnable}}\n  {{.UseLine}}{{end}}",
		"{{if .Runnable}}{{if not .HasAvailableSubCommands}}\n  {{.UseLine}}{{end}}{{end}}", 1)
	tmpl = strings.Replace(tmpl,
		"{{.CommandPath}} [command]",
		"{{.CommandPath}} [-h | command]", 1)
	root.SetUsageTemplate(tmpl)
	root.PersistentFlags().BoolVarP(&verbose, "verbose", "v", false,
		"enable verbose output")
	root.AddCommand(newShowCmd())
	root.AddCommand(newExportCmd())
	return root
}

func newShowCmd() *cobra.Command {
	var argNum int
	var scale float64

	cmd := &cobra.Command{
		Use:   "show [args...]",
		Short: "Show flag values and positional arguments",
		Long: strings.TrimSpace(`
Show the parsed flag values along with any positional arguments.
Demonstrates short/long flag aliases and required flags.`),
		Example: strings.TrimSpace(`
  flag show -n 4 foo bar
  flag show --arg-num 4 --scale 2.5 foo bar
  flag show -v -n 1 baz`),
		RunE: func(cmd *cobra.Command, args []string) error {
			fmt.Printf("arg-num = %d\n", argNum)
			fmt.Printf("scale   = %g\n", scale)
			if verbose {
				fmt.Println("verbose = true")
			}
			fmt.Printf("There are %d positional arguments:\n", len(args))
			for i, v := range args {
				fmt.Printf("  [%d] %s\n", i, v)
			}
			return nil
		},
	}

	cmd.Flags().IntVarP(&argNum, "arg-num", "n", 1,
		"which argument to select")
	cmd.Flags().Float64VarP(&scale, "scale", "s", 100,
		"a scaling factor")
	cmd.MarkFlagRequired("arg-num")
	return cmd
}

type Thang int

const (
	Unspecified Thang = iota
	Foo
	Bar
)

func (t *Thang) String() string {
	switch *t {
	case Foo:
		return "foo"
	case Bar:
		return "bar"
	}
	return "unspecified"
}

func (t *Thang) Set(s string) error {
	switch s {
	case "foo":
		*t = Foo
	case "bar":
		*t = Bar
	default:
		return fmt.Errorf("must be \"foo\" or \"bar\"")
	}
	return nil
}

func (t *Thang) Type() string {
	return "thang"
}

func newExportCmd() *cobra.Command {
	var useFoo bool
	var useBar bool

	cmd := &cobra.Command{
		Use:   "exclude",
		Short: "Show exclusivity (demonstrates mutually exclusive flags)",
		Example: strings.TrimSpace(`
  flag exclude --foo
  flag exclude --bar`),
		RunE: func(cmd *cobra.Command, args []string) error {
			var value Thang
			if useFoo {
				value = Foo
			} else if useBar {
				value = Bar
			}
			fmt.Printf("Selected: %s\n", &value)
			if verbose {
				fmt.Println("verbose = true")
			}
			return nil
		},
	}

	cmd.Flags().BoolVar(&useFoo, "foo", false, "use the foo")
	cmd.Flags().BoolVar(&useBar, "bar", false, "use the bar")
	cmd.MarkFlagsMutuallyExclusive("foo", "bar")
	return cmd
}

func main() {
	root := newRootCmd()
	if err := root.Execute(); err != nil {
		fmt.Fprintf(os.Stderr, "%s\n", err)
		os.Exit(1)
	}
}
