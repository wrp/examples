
default:
	cargo build

%: src/%.rs
	cargo run --bin $*

Cargo.toml: ./Cargo.toml.sh
	./Cargo.toml.sh > Cargo.toml
