/*
Demonstration of cobra for CLI flag parsing.

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
	}
	root.SetOut(os.Stdout)
	root.SetErr(os.Stderr)
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

func newExportCmd() *cobra.Command {
	var outFile string
	var toStdout bool
	var format string

	cmd := &cobra.Command{
		Use:   "export",
		Short: "Export data (demonstrates mutually exclusive flags)",
		Example: strings.TrimSpace(`
  flag export --stdout --format json
  flag export --output result.csv --format csv`),
		RunE: func(cmd *cobra.Command, args []string) error {
			if toStdout {
				fmt.Printf("Exporting to stdout in %s format\n", format)
			} else {
				fmt.Printf("Exporting to %s in %s format\n", outFile, format)
			}
			if verbose {
				fmt.Println("verbose = true")
			}
			return nil
		},
	}

	cmd.Flags().StringVarP(&outFile, "output", "o", "",
		"output file path")
	cmd.Flags().BoolVar(&toStdout, "stdout", false,
		"write to stdout instead of a file")
	cmd.Flags().StringVarP(&format, "format", "f", "json",
		"output format (json, csv, xml)")
	cmd.MarkFlagsMutuallyExclusive("output", "stdout")
	return cmd
}

func main() {
	root := newRootCmd()
	if err := root.Execute(); err != nil {
		fmt.Fprintf(os.Stderr, "%s\n", err)
		os.Exit(1)
	}
}
