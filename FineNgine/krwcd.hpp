/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl
 Copyright (C) 2003, 2004, 2005, 2006 StatPro Italia srl
 Copyright (C) 2006 Katiuscia Manzoni
 Copyright (C) 2006 Chiara Fornarola
 Copyright (C) 2009 Roland Lichters
 Copyright (C) 2009 Ferdinando Ametrano

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file krwcd.hpp
    \brief %KrwCD index
*/

#ifndef quantlib_krwcd_hpp
#define quantlib_krwcd_hpp

#include <ql/indexes/iborindex.hpp>

namespace QuantLib {

    //! %KrwCD index
    /*! KrwCD rate fixed by the ECB.

        \warning This is the rate fixed by the ECB. Use EurLibor
                 if you're interested in the London fixing by BBA.
    */
    class KrwCD : public IborIndex {
      public:
        KrwCD(const Period& tenor,
                const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>());
    };

    //! Actual/365 %KrwCD index
    /*! KrwCD rate adjusted for the mismatch between the actual/360
        convention used for KrwCD and the actual/365 convention
        previously used by a few pre-EUR currencies.
    */
    class KrwCD365 : public IborIndex {
      public:
        KrwCD365(const Period& tenor,
                   const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>());
    };

    //! 1-week %KrwCD index
    class KrwCDSW : public KrwCD {
      public:
        explicit KrwCDSW(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(1, Weeks), h) {}
    };

    //! 2-weeks %KrwCD index
    class KrwCD2W : public KrwCD {
      public:
        explicit KrwCD2W(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(2, Weeks), h) {}
    };

    //! 3-weeks %KrwCD index
    class KrwCD3W : public KrwCD {
      public:
        explicit KrwCD3W(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(3, Weeks), h) {}
    };

    //! 1-month %KrwCD index
    class KrwCD1M : public KrwCD {
      public:
        explicit KrwCD1M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(1, Months), h) {}
    };

    //! 2-months %KrwCD index
    class KrwCD2M : public KrwCD {
      public:
        explicit KrwCD2M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(2, Months), h) {}
    };

    //! 3-months %KrwCD index
    class KrwCD3M : public KrwCD {
      public:
        explicit KrwCD3M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(3, Months), h) {}
    };
	//! 91-days %KrwCD index
	class KrwCD91D : public KrwCD {
	public:
		explicit KrwCD91D(const Handle<YieldTermStructure>& h =
			Handle<YieldTermStructure>())
			: KrwCD(Period(91, Days), h) {}
	};
    //! 4-months %KrwCD index
    class KrwCD4M : public KrwCD {
      public:
        explicit KrwCD4M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(4, Months), h) {}
    };

    //! 5-months %KrwCD index
    class KrwCD5M : public KrwCD {
      public:
        explicit KrwCD5M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(5, Months), h) {}
    };

    //! 6-months %KrwCD index
    class KrwCD6M : public KrwCD {
      public:
        explicit KrwCD6M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(6, Months), h) {}
    };

    //! 7-months %KrwCD index
    class KrwCD7M : public KrwCD {
      public:
        explicit KrwCD7M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(7, Months), h) {}
    };

    //! 8-months %KrwCD index
    class KrwCD8M : public KrwCD {
      public:
        explicit KrwCD8M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(8, Months), h) {}
    };

    //! 9-months %KrwCD index
    class KrwCD9M : public KrwCD {
      public:
        explicit KrwCD9M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(9, Months), h) {}
    };

    //! 10-months %KrwCD index
    class KrwCD10M : public KrwCD {
      public:
        explicit KrwCD10M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(10, Months), h) {}
    };

    //! 11-months %KrwCD index
    class KrwCD11M : public KrwCD {
      public:
        explicit KrwCD11M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(11, Months), h) {}
    };

    //! 1-year %KrwCD index
    class KrwCD1Y : public KrwCD {
      public:
        explicit KrwCD1Y(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD(Period(1, Years), h) {}
    };


    //! 1-week %KrwCD365 index
    class KrwCD365_SW : public KrwCD365 {
      public:
        explicit KrwCD365_SW(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(1, Weeks), h) {}
    };

    //! 2-weeks %KrwCD365 index
    class KrwCD365_2W : public KrwCD365 {
      public:
        explicit KrwCD365_2W(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(2, Weeks), h) {}
    };

    //! 3-weeks %KrwCD365 index
    class KrwCD365_3W : public KrwCD365 {
      public:
        explicit KrwCD365_3W(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(3, Weeks), h) {}
    };

    //! 1-month %KrwCD365 index
    class KrwCD365_1M : public KrwCD365 {
      public:
        explicit KrwCD365_1M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(1, Months), h) {}
    };

    //! 2-months %KrwCD365 index
    class KrwCD365_2M : public KrwCD365 {
      public:
        explicit KrwCD365_2M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(2, Months), h) {}
    };

    //! 3-months %KrwCD365 index
    class KrwCD365_3M : public KrwCD365 {
      public:
        explicit KrwCD365_3M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(3, Months), h) {}
    };

    //! 4-months %KrwCD365 index
    class KrwCD365_4M : public KrwCD365 {
      public:
        explicit KrwCD365_4M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(4, Months), h) {}
    };

    //! 5-months %KrwCD365 index
    class KrwCD365_5M : public KrwCD365 {
      public:
        explicit KrwCD365_5M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(5, Months), h) {}
    };

    //! 6-months %KrwCD365 index
    class KrwCD365_6M : public KrwCD365 {
      public:
        explicit KrwCD365_6M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(6, Months), h) {}
    };

    //! 7-months %KrwCD365 index
    class KrwCD365_7M : public KrwCD365 {
      public:
        explicit KrwCD365_7M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(7, Months), h) {}
    };

    //! 8-months %KrwCD365 index
    class KrwCD365_8M : public KrwCD365 {
      public:
        explicit KrwCD365_8M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(8, Months), h) {}
    };

    //! 9-months %KrwCD365 index
    class KrwCD365_9M : public KrwCD365 {
      public:
        explicit KrwCD365_9M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(9, Months), h) {}
    };

    //! 10-months %KrwCD365 index
    class KrwCD365_10M : public KrwCD365 {
      public:
        explicit KrwCD365_10M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(10, Months), h) {}
    };

    //! 11-months %KrwCD365 index
    class KrwCD365_11M : public KrwCD365 {
      public:
        explicit KrwCD365_11M(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(11, Months), h) {}
    };

    //! 1-year %KrwCD365 index
    class KrwCD365_1Y : public KrwCD365 {
      public:
        explicit KrwCD365_1Y(const Handle<YieldTermStructure>& h =
                                    Handle<YieldTermStructure>())
        : KrwCD365(Period(1, Years), h) {}
    };

}

#endif
