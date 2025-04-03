resource local_file sub_module_file {
	for_each = toset(var.names)
	content = "Hello, World from file ${path.module}/${each.key}!\n"
	filename = "${path.module}/${each.key}"
}
