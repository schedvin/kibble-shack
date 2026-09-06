import { LitElement, html, css } from 'lit';
import { tailwindStyles } from './tailwind-styles.js';

// ==========================================
// 1. Connection Status Badge Component
// ==========================================
class ConnectionStatus extends LitElement {
  static properties = {
    connected: { type: Boolean }
  };

  static styles = [
    tailwindStyles,
    css`:host { display: inline-block; }`
  ];

  constructor() {
    super();
    this.connected = false;
  }

  render() {
    return this.connected ? html`
      <span class="inline-flex items-center gap-2 px-3 py-1.5 rounded-full text-xs font-medium bg-emerald-500/10 text-emerald-400 border border-emerald-500/20">
        <span class="w-2 h-2 rounded-full bg-emerald-400 animate-pulse"></span> Connected
      </span>
    ` : html`
      <span class="inline-flex items-center gap-2 px-3 py-1.5 rounded-full text-xs font-medium bg-rose-500/10 text-rose-400 border border-rose-500/20">
        <span class="w-2 h-2 rounded-full bg-rose-400"></span> Disconnected
      </span>
    `;
  }
}
customElements.define('connection-status', ConnectionStatus);

// ==========================================
// 2. LED Control Card Component
// ==========================================
class EspLedCard extends LitElement {
  static properties = {
    state: { type: String }
  };

  static styles = [
    tailwindStyles,
    css`:host { display: block; }`
  ];

  constructor() {
    super();
    this.state = 'Unknown';
  }

  async _toggleLED() {
    try {
      const res = await fetch('/api/led/toggle', { method: 'POST' });
      const data = await res.json();
      if (data.state) {
        this.state = data.state;
      }
    } catch (err) {
      console.error("Failed to toggle LED:", err);
    }
  }

  render() {
    return html`
      <div class="flex items-center justify-between bg-slate-700/50 p-6 rounded-lg border border-slate-600/50">
        <div>
          <div class="font-medium text-lg">Board LED</div>
          <div class="text-sm text-slate-400">State: ${this.state}</div>
        </div>
        <button 
          @click="${this._toggleLED}" 
          class="bg-emerald-500 hover:bg-emerald-600 active:scale-95 px-5 py-2.5 rounded-lg font-semibold transition">
          Toggle
        </button>
      </div>
    `;
  }
}
customElements.define('esp-led-card', EspLedCard);

// ==========================================
// 3. Sound Alert Notification Component
// ==========================================
class SoundAlert extends LitElement {
  static properties = {
    active: { type: Boolean }
  };

  static styles = [
    tailwindStyles,
    css`:host { display: block; }`
  ];

  constructor() {
    super();
    this.active = false;
    this.timeout = null;
  }

  trigger() {
    this.active = true;
    clearTimeout(this.timeout);
    this.timeout = setTimeout(() => {
      this.active = false;
    }, 1500);
  }

  render() {
    return html`
      <div class="${this.active ? 'flex' : 'hidden'} bg-amber-500/10 border border-amber-500/30 text-amber-300 p-5 rounded-lg text-sm items-center gap-3 transition">
        <svg class="w-6 h-6 text-amber-400 shrink-0" fill="none" stroke="currentColor" viewBox="0 0 24 24">
          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15.536 8.464a5 5 0 010 7.072M18.364 5.636a9 9 0 010 12.728M5.586 15H4a1 1 0 01-1-1v-4a1 1 0 011-1h1.586l4.707-4.707C10.923 3.663 12 4.109 12 5v14c0 .891-1.077 1.337-1.707.707L5.586 15z"></path>
        </svg>
        <span class="text-base font-medium">Sound Detected!</span>
      </div>
    `;
  }
}
customElements.define('sound-alert', SoundAlert);

// ==========================================
// 4. Main Controller Component
// ==========================================
class EspDashboard extends LitElement {
  static properties = {
    connected: { type: Boolean }
  };

  static styles = [
    tailwindStyles,
    css`:host { display: block; width: 100%; max-width: 28rem; }`
  ];

  constructor() {
    super();
    this.connected = false;
  }

  firstUpdated() {
    this._connectWebSocket();
  }

  _connectWebSocket() {
    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const wsUrl = `${protocol}//${window.location.host}/ws`;

    this.ws = new WebSocket(wsUrl);

    this.ws.onopen = () => {
      this.connected = true;
    };

    this.ws.onclose = () => {
      this.connected = false;
      setTimeout(() => this._connectWebSocket(), 2000);
    };

    this.ws.onmessage = (event) => {
      try {
        const data = JSON.parse(event.data);

        if (data.event === 'sound_detected') {
          this.shadowRoot.querySelector('sound-alert')?.trigger();
        }

        if (data.state !== undefined) {
          const ledCard = this.shadowRoot.querySelector('esp-led-card');
          if (ledCard) ledCard.state = data.state;
        }
      } catch (e) {
        console.log("Raw WS message:", event.data);
      }
    };
  }

  render() {
    return html`
      <div class="bg-slate-800 rounded-xl p-8 shadow-2xl border border-slate-700 flex flex-col gap-8">
        <div class="flex items-center justify-between">
          <h1 class="text-2xl font-bold text-emerald-400">ESP Controller</h1>
          <connection-status .connected="${this.connected}"></connection-status>
        </div>
        
        <div class="flex flex-col gap-6">
          <esp-led-card></esp-led-card>
          <sound-alert></sound-alert>
        </div>
      </div>
    `;
  }
}
customElements.define('esp-dashboard', EspDashboard);