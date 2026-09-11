# Add this file to ~/.config/opencode/plugins/notifications.js
# to emit a bel when the model responds


import { openSync, closeSync, writeSync } from "node:fs"

export const NotificationPlugin = async () => {
  return {
    event: async ({ event }) => {
      if (event.type !== "session.idle") return

      try {
        const fd = openSync("/dev/tty", "w")
        try {
          writeSync(fd, "\x07")
        } finally {
          closeSync(fd)
        }
      } catch {
        try {
          process.stdout.write("\x07")
        } catch {}
      }
    },
  }
}
