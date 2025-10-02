import { $, on } from './utils.js';

on(document, 'DOMContentLoaded', () => {
  const btn = $('#cta');
  if (btn) on(btn, 'click', () => alert('👍 Minimal, modular JS'));
});
