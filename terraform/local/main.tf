
# Create local file

terraform {
	required_version = ">1.0"
}

resource local_file a {
	content = "Hello, World!\n"
	filename = "${path.module}/hello"
}

# Create a local file using indent removing heredoc
# leading hard tabs are removed, other whitespace remains
# There cannot be whitesapce between '<<-' and the label name
resource local_file b {
	content = <<-EOF
		Hello, Heredoc
	EOF
	filename = "${path.module}/heredoc"
}

# Create a local file using non indent removing heredoc
# leading whitespace is retained, label can be indented
resource local_file c {
	content = <<EOF
		Hello, indented heredoc
	EOF
	filename = "${path.module}/nodent"
}
