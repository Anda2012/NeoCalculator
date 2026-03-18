import Link from 'next/link';
import { Calculator } from 'lucide-react';

export default function Navbar() {
  return (
    <div className="fixed top-6 left-1/2 -translate-x-1/2 z-50 w-[90%] max-w-4xl">
      <header className="rounded-full border border-white/10 bg-white/5 backdrop-blur-lg shadow-[0_0_15px_rgba(0,0,0,0.5)] supports-[backdrop-filter]:bg-white/5">
        <div className="px-6 h-14 flex items-center justify-between">
          <Link href="/" className="flex items-center gap-2 group">
            <Calculator className="w-5 h-5 text-[#ccff00] transition-transform group-hover:rotate-12" />
            <span className="font-bold text-white tracking-tight">NumOS</span>
          </Link>
          <nav className="hidden md:flex items-center gap-6 text-sm font-mono tracking-wider">
            <Link href="/manifesto" className="text-gray-400 hover:text-white transition-colors">The Mission</Link>
            <Link href="/compare" className="text-gray-400 hover:text-[#ccff00] transition-colors">Compare</Link>
            <Link href="/roadmap" className="text-gray-400 hover:text-white transition-colors">Roadmap</Link>
            <Link href="/docs" className="text-gray-400 hover:text-white transition-colors">Interactive Docs</Link>
          </nav>
          <div className="flex items-center gap-4">
            <a href="https://github.com/El-EnderJ/NeoCalculator" target="_blank" rel="noopener noreferrer" className="hidden sm:inline-block border border-white/20 hover:border-[#ccff00] hover:text-[#ccff00] text-xs font-mono px-4 py-1.5 rounded-full transition-colors">
              GITHUB
            </a>
          </div>
        </div>
      </header>
    </div>
  );
}
