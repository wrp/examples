
ifndef SP_PAGE
$(error SP_PAGE is not set)
endif

ifndef SP_TOKEN
$(error SP_TOKEN is not set)
endif


$(SP_PAGE)-components:
	curl -Ssf -X GET \
		-H "Authorization:OAuth $(SP_TOKEN)" \
		"https://api.statuspage.io/v1/pages/$(SP_PAGE)/components" > $@
