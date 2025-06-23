/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    "./src/**/*.{js,jsx,ts,tsx}",
    "./public/index.html"
  ],
  theme: {
    extend: {
      colors: {
        uber: {
          black: '#000000',
          white: '#FFFFFF',
          gray: {
            50: '#F7F7F7',
            100: '#E6E6E6',
            200: '#D4D4D4',
            300: '#A6A6A6',
            400: '#8C8C8C',
            500: '#737373',
            600: '#525252',
            700: '#404040',
            800: '#262626',
            900: '#171717'
          },
          green: {
            50: '#F0F9FF',
            100: '#E0F2FE',
            500: '#06B6D4',
            600: '#0891B2'
          }
        }
      },
      fontFamily: {
        sans: ['Inter', 'system-ui', 'sans-serif']
      },
      animation: {
        'fade-in': 'fadeIn 0.5s ease-in-out',
        'slide-up': 'slideUp 0.3s ease-out',
        'pulse-slow': 'pulse 2s cubic-bezier(0.4, 0, 0.6, 1) infinite'
      },
      keyframes: {
        fadeIn: {
          '0%': { opacity: '0' },
          '100%': { opacity: '1' }
        },
        slideUp: {
          '0%': { transform: 'translateY(10px)', opacity: '0' },
          '100%': { transform: 'translateY(0)', opacity: '1' }
        }
      },
      boxShadow: {
        'uber': '0 2px 8px rgba(0, 0, 0, 0.1)',
        'uber-lg': '0 4px 16px rgba(0, 0, 0, 0.1)'
      }
    },
  },
  plugins: [],
}
