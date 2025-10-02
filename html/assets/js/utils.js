export const $  = (sel, el = document) => el.querySelector(sel);
export const $$ = (sel, el = document) => [...el.querySelectorAll(sel)];
export const on = (el, ev, fn, opts) => el.addEventListener(ev, fn, opts);
