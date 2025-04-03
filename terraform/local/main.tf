

terraform {
	required_version = ">= 1.6, ~> 1.6.0, != 1.2"
	# ~> is the pessimistic constraint which allows only the rightmost
	#    number to differ
}

# Create 2 local files
resource local_file counted {
	count = 2
	content = "Hello, World from file ${count.index}!\n"
	filename = "${path.module}/hello.${count.index}"
}

# Create local files using indent removing heredoc
# leading hard tabs are removed, other whitespace remains
# There cannot be whitesapce between '<<-' and the label name
resource local_file iterated {
	for_each = toset(["foo", "bar"])

	content = <<-EOF
		Hello, Heredoc from ${each.key}
	EOF
	filename = "${path.module}/heredoc.${each.key}"
}

# Create a local file using non indent removing heredoc
# leading whitespace is retained, label can be indented
resource local_file c {
	content = <<EOF
		Hello, indented heredoc
	EOF
	filename = "${path.module}/nodent"
}

module "module" {
	source = "./module"
	names = ["foo", "bar", "baz"]
}

output "sample_content" {
	value = local_file.counted[1].content
}
