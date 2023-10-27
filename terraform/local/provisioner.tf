# Create a local file dynamically, using the local-exec provisioner
resource local_file dynamic {
	content = "hello, world!"
	filename = "dyn"
	provisioner "local-exec" {
		command = "printf ' %s\n' '${self.content}' >> ${self.filename}"
		on_failure = fail
	}
	# destroy provisioner does not execute.  Indeed, this file is not deleted
	# by tofu destroy.  See https://github.com/hashicorp/terraform/issues/13549
	provisioner "local-exec" {
		when = destroy
		command = "printf '${self.filename} destroyed!!\n' > output"
	}
	# provisionsers execute in the order specified here in the config file
	provisioner "local-exec" {
		when = create
		command = "date >> ${self.filename}; false"
		on_failure = continue

	}
}
