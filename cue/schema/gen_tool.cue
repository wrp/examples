package config

import (
	"encoding/yaml"
	"tool/file"
)

env: string | *"" @tag(env)

command: gen: {
	for envName, envData in environments {
		if env == "" || env == envName {
			"write-\(envName)": file.Create & {
				filename: "\(envName).yaml"
				// Marshal the CUE data structures cleanly into YAML strings
				contents: yaml.Marshal(envData)
			}
		}
	}
}
