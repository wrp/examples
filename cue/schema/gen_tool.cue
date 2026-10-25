package config

import (
	"encoding/yaml"
	"tool/file"
)

// Define it as a normal field with an empty string default
env: string | *"" @tag(env)

// This creates a custom executable workflow command named 'gen'
command: gen: {
	// Loop through every environment defined in config.cue
	for envName, envData in environments {
		// Dynamically generate a write task for each environment
		if env == "" || env == envName {
			"write-\(envName)": file.Create & {
				filename: "\(envName).yaml"
				// Marshal the CUE data structures cleanly into YAML strings
				contents: yaml.Marshal(envData)
			}
		}
	}
}
