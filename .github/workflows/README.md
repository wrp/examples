# Repository Configuration

Configure the repository with:

    # Allow PRs to be auto-merged when requirements are met
    gh repo edit --enable-auto-merge

    # Use merge commits (not squash or rebase) as the default merge strategy
    gh repo edit --enable-merge-commit

Protect the main branch (require PRs, CI checks, no admin bypass):

    gh api repos/${owner}/${repo}/branches/main/protection \
        --method PUT \
        --field required_status_checks='{"strict":true,"contexts":[]}' \
        --field enforce_admins=true \
        --field required_pull_request_reviews='{"required_approving_review_count":0}' \
        --field restrictions=null  # no restrictions on who can merge PRs
