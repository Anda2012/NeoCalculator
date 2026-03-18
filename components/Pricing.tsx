'use client';

import { motion } from 'framer-motion';

const devices = [
  { name: "HP Prime", price: "$180", highlight: false },
  { name: "TI-84 Plus CE", price: "$150", highlight: false },
  { name: "NumWorks", price: "$125", highlight: false },
  { name: "NeoCalculator", price: "$15", highlight: true },
];

const containerVariants = {
  hidden: { opacity: 0 },
  visible: {
    opacity: 1,
    transition: {
      staggerChildren: 0.15
    }
  }
};

const cardVariants = {
  hidden: { opacity: 0, y: 30 },
  visible: { 
    opacity: 1, 
    y: 0,
    transition: { type: 'spring', stiffness: 300, damping: 24 }
  }
};

export default function Pricing() {
  return (
    <section className="bg-[#0a0a0a] relative py-20 px-4 sm:px-6">
      <div className="max-w-5xl mx-auto">
        <h2 className="text-3xl md:text-5xl font-black mb-12 text-center tracking-tighter text-white">
          THE <span className="text-[#ccff00]">MONOPOLY</span> IS OVER
        </h2>
        <motion.div 
          variants={containerVariants}
          initial="hidden"
          whileInView="visible"
          viewport={{ once: true, margin: "-100px" }}
          className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-6"
        >
          {devices.map((device) => (
            <motion.div 
              key={device.name}
              variants={cardVariants}
              className={`glass-panel p-8 flex flex-col items-center justify-center text-center transition-all duration-300 ${
                device.highlight 
                  ? 'border-[#ccff00]/50 shadow-[0_0_30px_rgba(204,255,0,0.15)] transform hover:-translate-y-2' 
                  : 'hover:border-white/20 hover:bg-white/10'
              }`}
            >
              <h3 className={`text-xl font-bold mb-4 font-mono ${device.highlight ? 'text-[#ccff00]' : 'text-gray-300'}`}>
                {device.name}
              </h3>
              <p className={`text-4xl md:text-5xl font-black ${device.highlight ? 'text-white' : 'text-gray-500'}`}>
                {device.price}
              </p>
            </motion.div>
          ))}
        </motion.div>
      </div>
    </section>
  );
}
