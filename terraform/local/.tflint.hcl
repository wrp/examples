# This file configures tflint.

rule "terraform_required_providers" {
  enabled = false
  source = true
  version = true
}
