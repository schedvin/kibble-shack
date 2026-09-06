import { css, unsafeCSS } from 'lit';
import tailwindCss from './style.css?inline';

// Wraps pre-compiled Tailwind CSS in Lit's CSS template literal
export const tailwindStyles = css`${unsafeCSS(tailwindCss)}`;