

variable "name" {
	type = string
	default = "hello"
}

resource local_file sub_module_file {
	content = "Hello, World from file ${path.module}/${var.name}!\n"
	filename = "${path.module}/${var.name}"
}
