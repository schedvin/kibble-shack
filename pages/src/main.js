import { LitElement, html, css } from 'lit';
import { tailwindStyles } from './tailwind-styles.js';

const SERVICE_UUID = "1710ccf7-8e1b-4e1d-9070-d359796f0a35"
const CREDENTIALS_UUID = "14c7022c-54a8-4259-8097-5ac2e5cc7bf4" // Fixed variable name
const IP_ADDRESS_UUID = "a1b2c3d4-e5f6-4a5b-8c9d-0e1f2a3b4c5d"

class KibbleShackProvisioning extends LitElement {
  constructor() {
    super();
  }

  static styles = [
    tailwindStyles,
    css`:host { display: block; width: 100%; max-width: 28rem; }`
  ];

  firstUpdated() {
    this.setupEventListeners();
  }

  render() {
    return html`
      <div class="bg-slate-800 rounded-2xl p-8 shadow-2xl border border-slate-700 flex flex-col gap-6 font-sans">
        
        <div>
          <h2 class="text-2xl font-bold text-white mb-1">KibbleShack Setup</h2>
          <p class="text-slate-400 text-sm">Provision your device over Bluetooth.</p>
        </div>

        <div class="flex flex-col gap-4">
          <div>
              <label class="block text-sm font-medium text-slate-300 mb-1.5">WiFi Network (SSID)</label>
              <input type="text" id="ssidInput" placeholder="MyHomeNetwork"
                     class="w-full px-4 py-2.5 bg-slate-900 border border-slate-600 rounded-lg text-white placeholder-slate-500 focus:outline-none focus:border-blue-500 focus:ring-1 focus:ring-blue-500 transition-colors">
          </div>
          
          <div>
              <label class="block text-sm font-medium text-slate-300 mb-1.5">WiFi Password</label>
              <input type="password" id="passInput" placeholder="SecretPassword"
                     class="w-full px-4 py-2.5 bg-slate-900 border border-slate-600 rounded-lg text-white placeholder-slate-500 focus:outline-none focus:border-blue-500 focus:ring-1 focus:ring-blue-500 transition-colors">
          </div>
          
          <div>
              <label class="block text-sm font-medium text-slate-300 mb-1.5">Device Hostname (mDNS)</label>
              <input type="text" id="hostInput" value="kibbleshack"
                     class="w-full px-4 py-2.5 bg-slate-900 border border-slate-600 rounded-lg text-white placeholder-slate-500 focus:outline-none focus:border-blue-500 focus:ring-1 focus:ring-blue-500 transition-colors">
          </div>
        </div>

        <button id="connectBtn" 
                class="w-full bg-blue-600 hover:bg-blue-500 active:bg-blue-700 text-white font-semibold py-3 px-4 rounded-lg shadow-lg shadow-blue-500/20 transition-all transform active:scale-[0.98]">
            Connect & Provision
        </button>
        
        <!-- Hidden by default, revealed when provisioning starts -->
        <div id="statusBox" class="hidden p-4 rounded-r-lg bg-slate-900 border-l-4 text-sm leading-relaxed tracking-wide">
        </div>
      </div>
  `;
  }

  setupEventListeners() {
    const btn = this.shadowRoot.getElementById('connectBtn');
    btn.addEventListener('click', () => this.handleProvisioning());
  }

  updateStatus(message, type = 'normal') {
    const box = this.shadowRoot.getElementById('statusBox');
    
    // Base classes applied to every state
    let classes = 'block p-4 rounded-r-lg bg-slate-900 border-l-4 text-sm leading-relaxed tracking-wide transition-all ';
    
    // Apply dynamic colors based on state
    if (type === 'error') {
        classes += 'border-red-500 text-red-400';
    } else if (type === 'success') {
        classes += 'border-green-500 text-green-400';
    } else {
        classes += 'border-blue-500 text-blue-300';
    }
    
    box.className = classes;
    box.innerHTML = message;
  }

  async handleProvisioning() {
    const ssid = this.shadowRoot.getElementById('ssidInput').value;
    const password = this.shadowRoot.getElementById('passInput').value;
    const hostname = this.shadowRoot.getElementById('hostInput').value.trim() || 'kibbleshack';

    if (!ssid) {
      this.updateStatus('Please enter a WiFi SSID.', 'error');
      return;
    }

    try {
      this.updateStatus('Requesting Bluetooth Device...');

      const device = await navigator.bluetooth.requestDevice({
        filters: [{ services: [SERVICE_UUID] }]
      });

      this.updateStatus('Connecting to device...');
      const server = await device.gatt.connect();
      const service = await server.getPrimaryService(SERVICE_UUID);

      // 1. Subscribe to the IP Characteristic
      const ipCharacteristic = await service.getCharacteristic(IP_ADDRESS_UUID);
      await ipCharacteristic.startNotifications();

      ipCharacteristic.addEventListener('characteristicvaluechanged', (event) => {
        const decoder = new TextDecoder('utf-8');
        const ipAddress = decoder.decode(event.target.value);

        if (ipAddress === "ERROR") {
          this.updateStatus('Failed to connect to WiFi. Check credentials.', 'error');
        } else {
          // Added Tailwind classes directly to the generated HTML links
          this.updateStatus(`
              <strong class="text-green-300 text-base block mb-2">✅ Connected to WiFi!</strong>
              <div class="flex flex-col gap-1">
                  <span>Direct IP: <a href="http://${ipAddress}" class="text-blue-400 hover:text-blue-300 underline underline-offset-2">http://${ipAddress}</a></span>
                  <span>mDNS: <a href="http://${hostname}.local" class="text-blue-400 hover:text-blue-300 underline underline-offset-2">http://${hostname}.local</a></span>
              </div>
              <em class="block mt-3 text-slate-400">Redirecting in 5 seconds...</em>
          `, 'success');

          setTimeout(() => {
            window.location.href = `http://${ipAddress}`;
          }, 5000);
        }
      });

      // 2. Send the WiFi credentials
      this.updateStatus('Sending credentials...');
      const credsCharacteristic = await service.getCharacteristic(CREDENTIALS_UUID);
      const payload = `${ssid};;;${password};;;${hostname}`;
      const encoder = new TextEncoder('utf-8');

      await credsCharacteristic.writeValue(encoder.encode(payload));
      this.updateStatus('Credentials sent! Waiting for device to connect to WiFi...');

    } catch (error) {
      console.error('BLE Error:', error);
      this.updateStatus(`Error: ${error.message}`, 'error');
    }
  }
}

customElements.define('kibble-shack-provisioning', KibbleShackProvisioning);