# Repository Configuration

Configure the repository with:

    # Allow PRs to be auto-merged when requirements are met
    gh repo edit --enable-auto-merge

    # Use merge commits (not squash or rebase) as the default merge strategy
    gh repo edit --enable-merge-commit

Protect the main branch (require PRs, CI checks, no admin bypass):

    gh api repos/${owner}/${repo}/branches/main/protection \
        --method PUT \
        --input - <<'EOF'
    {
      "required_status_checks": {"strict": true, "contexts": []},
      "enforce_admins": true,
      "required_pull_request_reviews": {"required_approving_review_count": 1},
      "restrictions": null
    }
    EOF

# GitHub Limitations

GitHub does not support true fast-forward merges. Even "rebase merge" rewrites
commits, changing their hashes. This means a single-commit branch based directly
on main cannot be merged without rewriting the commit.

A reasonable workflow would allow preserving commit hashes when a fast-forward
merge is possible (i.e., `git merge --ff-only`), but GitHub forces all merges
through the web UI to rewrite history.
