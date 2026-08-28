# tmux status-right: trailing duplicated characters with a different background


This issue is not fixed.  Afer increasing the font size in the terminal,
the stray characters appear to the right of the status window.  Which 2
characters from the status string are duplicated changes depending on the
font size.  This is currently happening in 267746603a4ff25baad62e3477b126411e49b52f

## Incident

In the tmux status bar, the right-side format

    set -g status-right '\
        #[fg=#(tmux-status-color)]#(tmux-status-prefix) \
        #[fg=white]#(hostname | cut -c1-17) %b-%d#{?client_prefix,#[reverse],} %H:%M%z'

rendered in the bottom-right corner as e.g. `10:07-060060`, where the last
two characters ("60") appeared with a background different from the rest of
the status right area. Appending test text to the format reproduced it more
clearly: a format ending in `…%H:%M%z AB CDE` displayed as
`…10:07-0600 AB CDECD`.

## Environment

- macOS (Darwin), Apple Silicon, build of tmux from `master` (00b5323d)
- Client geometry: 241x71 (`client_width=241`), `tmux-256color`, UTF-8
- Buggy server: `tmux 3.7b-673-ge3915f83`
- Fixed server: `tmux 3.7b-802-g7fd437a1`
- Session had ~8 windows; window titles truncated to `#{=20:…}` per window
  by `set-tmux-width` (a window-status-format script driven by
  `window-created`/`window-closed` hooks)
- Relevant options: `status-right-length 40` (default), `status-justify left`
  (default), `status-style bg=black`, `window-status-current-style
  fg=white,bold,bg=yellow`

## Verified during diagnosis

1. **The format text itself was clean.** `tmux display-message -p` with the
   fully expanded `status-right` produced exactly
   `Aug-27 10:14-0600 AB CDE` — no duplicated characters, no spurious
   content. tmux's own expansion is a trustworthy oracle for "what text is
   in the status line"; anything extra on screen is a rendering defect,
   not a format problem.
2. **`%z` is not the source.** `%H:%M%z` expands to `HH:MM±HHMM`
   (`11:52-0600` for UTC-06:00 on this machine). The "60" was the tail of
   the UTC offset, duplicated, not an extra field.
3. **Locale/helpers ruled out.** `tmux-status-prefix`/`tmux-status-color`
   scripts and `pmset -g ps` output were checked; nothing printed stray
   digits.
4. **Changing the status option live changes the bar immediately** — no
   server restart is needed for `status-right` experiments.
5. **The defect is build-specific.** Same config, same machine:
   - `3.7b-673-ge3915f83` server → duplicated tail chars with a foreign
     background (the window-list cell background, i.e. a leftover repaint
     artifact where the window list meets the right-status region).
   - `3.7b-802-g7fd437a1` server → clean render, no duplication.
   - Also observed on the old build: clearing the built-in `status-format`
     global (`tmux set -g status-format ''`, which disables the newer
     `align=`/`list=` status renderer and falls back to the legacy
     `status-left`/`status-right` path) also made the artifact disappear.

## Hypothesis (not conclusively pinned down)

The 673-era master build contained a drawing bug in the newer status-bar
layout engine (the one driven by the `status-format` global using `align=`
/ `list=` markup): a sliver of the window-list region was left unpainted
at the boundary with the right status, so leftover cells (with the window
list's background colour) showed through past the last character of
`status-right`. Either the 802 build fixed the drawing or it changed
behaviour such that the overlap no longer occurs. It likely only manifests
where the window list runs long (many windows / long titles) and touches
the right-status area.

## Pitfalls to watch for if this recurs

1. **A binary upgrade does NOT change a running server.** All tmux clients
   attach to the existing server process; status rendering is done
   server-side. After rebuild, you must
   `tmux kill-server` and re-attach, and that terminates every session and
   everything running inside them. Check which server is actually running
   with `tmux display-message -p '#{version_full}'`, not `tmux -V`.
2. **Distinguish format text from painted pixels.** Before suspecting
   `status-right`/formats, expand them:
   `tmux display-message -p "$(tmux show-options -g status-right | sed 's/^status-right //')"`.
   If the expansion is clean, the problem is in drawing, not in your config.
3. **The status bar has three painted regions** — status-left, the window
   list, and status-right — and any anomaly *between* them is almost
   certainly an overlap/repaint defect, not a trailing character in your
   format.
4. **Workarounds that worked on the old build:**
   - `tmux set -g status-format ''` (disable the new status renderer
     globally), or
   - upgrade to a fixed `master` build and restart the server.
5. **Capture a real render for bisection.** Nested attach +
   `capture-pane` is unreliable in scripts (client-attached hooks like
   `pkill -WINCH bash` run, `attach-session` takes no target argument, and
   `exit-empty` kills a server whose last window closes). Reliable options:
   `script -q log sh -c 'tmux attach…'` on a pty-sized terminal, or attach
   from a real terminal at the exact geometry where the problem appears
   (here: 241 columns).
6. **`status-justify` matters.** This config used the default `left`, so
   the window list starts left and grows right toward status-right; if the
   defect ever looks like list-vs-right-status overlap, `status-justify
   centre` or shrinking window titles (`#{=N:…}`) changes the geometry and
   tells you whether overlap is involved.
