
package top

import "wrp.example/top/sub:pkg_name"

config: pkg_name.#Config & {
	appName: "alpha"
	port:    80
	features: logging: true
}
