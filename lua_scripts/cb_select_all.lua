-------------------------------------------------------------------------------
--
-- $BeginLicense$
--
-- (C) 2015-2021 by Camiel Bouchier (camiel@bouchier.be)
--
-- This file is part of cb_find_duplicates.
-- All rights reserved.
-- You are granted a non-exclusive and non-transferable license to use this
-- software for personal or internal business purposes.
--
-- THIS SOFTWARE IS PROVIDED "AS IS" AND
-- ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
-- WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
-- DISCLAIMED. IN NO EVENT SHALL Camiel Bouchier BE LIABLE FOR ANY
-- DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
-- (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
-- LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
-- ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
-- (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
-- SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--
-- $EndLicense$
--
-------------------------------------------------------------------------------

title       = "All minus first"
description = "Select all minus first"

function select(files, mtimes, ctimes, inodes, size)
  -- files : table of filenames.
  -- mtimes : corresponding table of modification times.
  -- ctimes : corresponding table of creation times.
  -- inodes : corresponding table of (fake) inodes.
  -- size  : filesize.
  local selected = {}
  for i=2, #files do
    selected[i] = true
  end
  return selected
end

-------------------------------------------------------------------------------
